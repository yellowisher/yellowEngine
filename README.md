# yellowEngine

yellowEngine is 3D game engine written in C++.  
It uses OpenGL and other cross-platform libraries but tested only in windows.  
And there is yellowEditor, WYSIWYG scene editor for the engine.

## Features
* System
  - Component System
  * Custom component reflection
  * OBB, Sphere Collision detection
    + AABB SAP broadphase detection
    * AABB BVH broadphase detection 
  * Scene loading/saving
* Rendering
  * Material (diffuse, specular, normal map)
  * Deferred Shading
    * Light volume stencil culling
  * Shadow Mapping
  * Skeletal animation, transition blending
  * Bloom (+HDR)

## Build
You have to build all dependency libraries and include them.  
I'm going to create cmake project someday.

## Dependencies
[glad](https://github.com/Dav1dde/glad)  
[GLFW](https://github.com/glfw/glfw)  
[jsoncpp](https://github.com/open-source-parsers/jsoncpp)  
[stb-image](https://github.com/nothings/stb)  
[Assimp](https://github.com/assimp/assimp)  
[imgui](https://github.com/ocornut/imgui) (yellowEditor only)

## Demo
#### yellowEditor  
<img src="https://github.com/yellowisher/yellowEngine/blob/master/Resources/editor.png" width="600"></img>  
#### Demo game  
![](https://github.com/yellowisher/yellowEngine/blob/master/Resources/game.gif)
