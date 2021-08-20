#shader vertex
#version 440 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 vi_color;
out vec3 vo_color;
void main(void)
{
	gl_Position = vec4(position, 0.0, 1.0);
	vo_color = vi_color;
}

#shader fragment
#version 440 core
in vec3 vo_color;
out vec3 color;
void main(void)
{
	color = vo_color;
}