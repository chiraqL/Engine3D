<p><h1 align="center">Engine3D</h1></p>
<p align="center">
  <a href="https://chiraql.github.io/Engine3D/docs/reference/index.html"><img src="https://img.shields.io/badge/Engine3D-docs-purple.svg?style=flat&logo=c%2B%2B" alt="Docs"></img></a>
  <a href="https://www.codacy.com/gh/chiraqL/Engine3D/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=chiraqL/Engine3D&amp;utm_campaign=Badge_Grade"><img src="https://app.codacy.com/project/badge/Grade/cbd00607ad0046649daeadeece20dd43"/></a>
  <a href="https://github.com/chiraqL/Engine3D/graphs/contributors"><img src="https://img.shields.io/badge/contributors-4-GREEN.svg" alt="contributors"></img></a>
  <a href="https://github.com/chiraqL/Engine3D/blob/main/LICENSE"><img src="https://badgen.net/github/license/Naereen/Strapdown.js" alt="License"></img></a>
</p>
<p align="center">
  <a href="https://www.microsoft.com/en-np/d/windows-10-home/d76qx4bznwk4?rtc=1"><img src="https://badgen.net/badge/icon/windows?icon=windows&label" alt="os"></img></a>
  <a href="https://visualstudio.microsoft.com"><img src="https://badgen.net/badge/icon/visualstudio?icon=visualstudio&label" alt="visual studio"></img></a>
</p>

Engine-3D is a 3D-Graphics Renderer built from scratch in C++, as an Undergraduate Computer Engineering Project for 5th Semester, assigned by [**Department of Electronics and Computer Engineering, Pulchowk Campus** ](http://doece.pcampus.edu.np/) under the course _Computer Graphics (EX603)_

## Features
- Model Transformation: Translation, Rotation, Scaling
- Orthographic & Perspective projection
- Line drawing: Bresenham's algorithm
- Rasterization: Scanline algorithm
- Wireframe, Color fill & Texture fill
- Model loading using .obj file
- Material rendering using .mtl file
- Look-at Camera & its movement
- Directional Light & its movement
- Z-Buffer
- Back-face Culling
- Polygon Clipping
- Local Illumination
- Phong Illumination Model
- Flat & Gouraud Shading

<p align="center">
<img src="https://github.com/chiraqL/Engine3D/blob/main/docs/Demo.gif?raw=true">
</p>



## Dependencies
- [**GLEW** ](http://glew.sourceforge.net//)
- [**GLFW** ](https://github.com/glfw/glfw/)
- [**ImGUI** ](https://github.com/ocornut/imgui/)
- [**assimp** ](https://github.com/assimp/assimp/)
- [**stb_image** ](https://github.com/nothings/stb/)

## Future Works
- Shadows
- Phong Shading
- Transparency
- Global Illumination
- Arcball and FPS Camera
- Anti-aliasing (FXAA)
- Multithreading
- Ray Tracing

## How to run
You will need Windows and Visual Studio to run.

Clone the repo:

`git clone https://github.com/chiraqL/Engine3D`

Open Engine3D.sln using Visual Studio

Add to Library Linker:

`assimp-vc142-mtd.lib;glew32.lib,glfw3.lib`

Add .dll files from /dependencies to .exe location

Build and Run.

## Project Members
-   075BCT015 - [**Arpan Pokharel** ](https://github.com/Naearp777/)
-   075BCT032 - [ **Chirag Lamsal** ](https://github.com/chiraqL/)
-   075BCT097 - [**Bibek Basyal**](https://github.com/Bibek99/)
-   075BCT099 - [**Saugat Kafley**](https://github.com/Saugatkafley/)