#pragma once

#include "glDraw.h"

void glDraw::init(void)
{
    zbuffer = new ZBuffer(vp_width, vp_height);     // Zbuffer
    pbuffer = new PBuffer(vp_width, vp_height);     // pixel/color buffer
    pbuffer->Clear(bgCol);                          // Clear color buffer to bg color

    //Check if Opengl version is at least 4.4 : to use persistent mapping
    const GLubyte* glVersion(glGetString(GL_VERSION));
    int major = glVersion[0] - '0';
    int minor = glVersion[2] - '0';
    if (major < 4 || minor < 4)
    {
        std::cerr << "ERROR: Minimum OpenGL version required is 4.4. Your current version is " << major << "." << minor << std::endl;
        exit(-1);
    }

    // Initialize GLEW-opengl
    glewExperimental = true;                        // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
    }

	vao = 0; vbo = 0; program = 0;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // not required
    glViewport(0, 0, 900, 900);             // gl draw window size
    glEnable(GL_DEPTH_TEST);                // not required
    glEnable(GL_TEXTURE_2D);                // not required
    initShader();   // initialize Vertex and Fragment shaders: VS is sent each pixels+color to draw

    //Create a vertex buffer object
    GLCall(glGenVertexArrays(1, &vao));     // create one vao
    GLCall(glBindVertexArray(vao));         // select that vao
    GLCall(glGenBuffers(1, &vbo));          // create one vbo
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));// select that vbo: vao -> vbo

    // attribute of vbo: vao -> vbo ->      x0, y0,     r0, g0, b0,     x1, y1,     r1, g1, b1 ...
    /*                                      attrib(0)   attrib(1)       attrib(0)   attrib(1)  ...
                                            |1___2______3___4___5_______| = stride1
                                           || = offset1

                                                        |1___2___3_______4___5______| = stride2
                                            |1___2______| = offset2
    */
    // x, y
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0));// stride1 = 5, offset1 = 0
    // r, g, b
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(2 * sizeof(float)))); // stride2 = 5, offset2 = 2

    //Create an immutable data store for the buffer
    size_t bufferSize(vp_width * vp_height * 5 * sizeof(float));   // attributes of pixels in viewport

    // For double/triple buffering
    //buffersize = buffersize * 2;  //double buffering
    //gSyncRanges[0].begin = 0;
    //gSyncRanges[1].begin = vp_width * vp_height * 5;
    //gSyncRanges[2].begin = vp_width * vp_height * 5 * 2;

    // persistently map buffer
    GLbitfield flags = GL_MAP_WRITE_BIT |
        GL_MAP_PERSISTENT_BIT |
        GL_MAP_COHERENT_BIT;

    GLCall(glBufferStorage(GL_ARRAY_BUFFER, bufferSize, 0, flags));     // gpu hint to allocate buffer

    // Map the gpu buffer persistently
    pMapBuffer = (float*)glMapBufferRange(GL_ARRAY_BUFFER,
        0,
        bufferSize,
        flags);

    // initialize x, y from 0,0 to w, h -> -1,1 to 1,-1
    //                  x0, y0,     r0, g0, b0,     x1, y1,     r1, g1, b1 ...  xw, yh
    //                  -1, 1,      r0, g0, b0,   -.99, .99     r1, g1, b1 ...  1 ,-1
    for (int xx = 0; xx < vp_width; ++xx)
    {
        for (int yy = 0; yy < vp_height; yy++)
        {
            pMapBuffer[(yy * vp_width + xx) * 5 + 0] = 2.0f * xx / vp_width - 1.0f;     // x
            pMapBuffer[(yy * vp_width + xx) * 5 + 1] = 1.0f - 2.0f * yy / vp_height;    // y
        }
    }
}
void glDraw::initShader()
{
    ShaderSource source = parseShader("src\\sh\\point.shader");

    // create shader program
    unsigned int program = createShader(source.vs, source.fs);
    GLCall(glUseProgram(program));
}
void glDraw::clear()
{
    zbuffer->Clear();       // reset zbuffer to 0,0,0, ...
    pbuffer->Clear(bgCol);  // reset color buffer to bg color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //not required
}
struct ShaderSource glDraw::parseShader(const std::string& path)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream stream(_SOLUTIONDIR_ + path);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}
unsigned int glDraw::compileShader(unsigned int type, const std::string& src)
{
    unsigned int id = glCreateShader(type);
    const char* source = src.c_str();
    GLCall(glShaderSource(id, 1, &source, nullptr));
    GLCall(glCompileShader(id));

    // Handling errors
    int res;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &res));
    //std::cout << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader compile status: " << res << std::endl;
    if (res == GL_FALSE)
    {
        int len;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
        char* message = (char*)alloca(len * sizeof(char));
        GLCall(glGetShaderInfoLog(id, len, &len, message));
        std::cout
            << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
            << "shader"
            << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}
unsigned int glDraw::createShader(const std::string& vs, const std::string& fs)
{
    // Create shader program
    unsigned int program = glCreateProgram();
    unsigned int vsID = compileShader(GL_VERTEX_SHADER, vs);
    unsigned int fsID = compileShader(GL_FRAGMENT_SHADER, fs);

    GLCall(glAttachShader(program, vsID));
    GLCall(glAttachShader(program, fsID));

    GLCall(glLinkProgram(program));

    GLint program_linked;

    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &program_linked));
    //std::cout << "Program link status: " << program_linked << std::endl;
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        GLCall(glGetProgramInfoLog(program, 1024, &log_length, message));
        std::cout << "Failed to link program" << std::endl;
        std::cout << message << std::endl;
    }

    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vsID));
    GLCall(glDeleteShader(fsID));

    return program;
}
void glDraw::putpixel(int x, int y, float z, col3& col) {
    x += vp_width / 2;       // View coordinate to monitor coordinate
    y = vp_height / 2 - y;   // translate to center and reflect about X axis

    // assertion
    if (x < 0 || x >= vp_width || y < 0 || y >= vp_height)
        return;
    if (zbuffer->TestAndSet(x, y, z))   // test depth with depth in zbuffer
        pbuffer->set(x, y, col);        // put in pixel/color buffer
}

void glDraw::update() {
    waitBuffer();   // stall GPU buffer
    // update rgb values in mapped buffer
    for (int xx = 0; xx < vp_width; ++xx)
    {
        for (int yy = 0; yy < vp_height; yy++)
        {
            if (pbuffer->tAt(xx, yy))
            {
                col3& icol = pbuffer->At(xx, yy);
                pMapBuffer[(yy * vp_width + xx) * 5 + 2] = icol.r;
                pMapBuffer[(yy * vp_width + xx) * 5 + 3] = icol.g;
                pMapBuffer[(yy * vp_width + xx) * 5 + 4] = icol.b;
            }
           else
            {
                pMapBuffer[(yy * vp_width + xx) * 5 + 2] = bgCol.r;
                pMapBuffer[(yy * vp_width + xx) * 5 + 3] = bgCol.g;
                pMapBuffer[(yy * vp_width + xx) * 5 + 4] = bgCol.b;
            }
        }
    }
    glPointSize(2);     // size of point (defaults to 1): not required
    glDrawArrays(GL_POINTS, 0, vp_width * vp_height);       // draw points on window
    lockBuffer();   //free GPU buffer
}

//opengl error handling
void glDraw::GLClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}
bool glDraw::GLLogError() {
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] ";
        switch (error) {
        case GL_INVALID_ENUM:
            std::cout << "GL_INVALID_ENUM : An unacceptable value is specified for an enumerated argument.";
            break;
        case GL_INVALID_VALUE:
            std::cout << "GL_INVALID_OPERATION : A numeric argument is out of range.";
            break;
        case GL_INVALID_OPERATION:
            std::cout << "GL_INVALID_OPERATION : The specified operation is not allowed in the current state.";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION : The frame-buffer object is not complete.";
            break;
        case GL_OUT_OF_MEMORY:
            std::cout << "GL_OUT_OF_MEMORY : There is not enough memory left to execute the command.";
            break;
        case GL_STACK_UNDERFLOW:
            std::cout << "GL_STACK_UNDERFLOW : An attempt has been made to perform an operation that would cause an internal stack to underflow.";
            break;
        case GL_STACK_OVERFLOW:
            std::cout << "GL_STACK_OVERFLOW : An attempt has been made to perform an operation that would cause an internal stack to overflow.";
            break;
        default:
            std::cout << "Unrecognized error" << error;
        }
        std::cout << std::endl;
        return false;
    }
    return true;
}
void glDraw::lockBuffer()
{
    if (gSync)
    {
        glDeleteSync(gSync);
    }
    gSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void glDraw::resize(unsigned int ww, unsigned int wh, unsigned int pw, unsigned int ph)
{
    win_width = ww;
    win_height = wh;
    pan_width = pw;
    pan_height = ph;
}

void glDraw::waitBuffer()
{
    if (gSync)
    {
        while (1)
        {
            GLenum waitReturn = glClientWaitSync(gSync, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
            if (waitReturn == GL_ALREADY_SIGNALED || waitReturn == GL_CONDITION_SATISFIED)
                return;
        }
    }

}
void glDraw::close(void)
{
    //Clean-up
    glUseProgram(0);
    glDeleteProgram(program);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glDeleteBuffers(1, &vbo);
    delete[] pbuffer;
	delete[] pMapBuffer;
    delete zbuffer;
}
