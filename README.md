# Soul
Soul Engine is a 3D renderer made with OpenGL from scratch in C++ for the end of the degree project at the university CITM (UPC), in the degree Videogame Design and development.

The main idea behind the renderer is to create an advanced way of visualizing models as well as a little tool to paint colors into the 3D model and export the textures to then use into another software like Unity. The project itself helps us to understand how to create a renderer with shaders from scratch and write a documentation for other programmers that want to also start on the subject of 3D painting and creating renderers, to have an easier time making them.

The renderer features a lot of techniques that the OpenGL renderer makes possible like gamma correction, HDR, Bloom, normal mapping, etc... And an easy way of importing models to quickly start using the software.

## Viewport Controls

Most of the menus from the engine are dockable and can be moved all around however the user likes to. The renderer comes with a default layout, similar to Unity, but can be modified.
All the menus are also interactuable, and most of them are self-explainatory, but we highly recommend testing all of them to understand the renderer itself and what is capable of.

## Controls

  - Orbit: Alt + Left mouse Button
  - Move camera: Right mouse button
  - Move forward or back: Mouse Scroll
  - Zoom in or back: Alt + right mouse button
  - Pan: Middle mouse button
  - Move light source (Directional light): Shift + Left mouse button
  - Increase/decrease brush size: Ctrl + Left mouse button
 
## Paint tool

To paint a model, the user needs to import a model from the "File" menu. After that, the model will appear in the center of the viewport. The renderer is made so the visualizer is the first thing that the users see. To activate the paint mode, the user needs to go to the edit tab, go to the render mode and choose "Paint Mode". After that he can paint the model by clicking on it.
The brush options are on the right, below the inspector menu.

Finally, when the user is happy with the result, he can export the texture in the "File" menu and selecting the desired folder. The texture is exported then and can be directly used in other softwares like Unity, which was the software used to test the results of the paint tool.

## Reference

Readers must be warned that the base system of the engine was created following the series of ["Game Engine"](https://www.youtube.com/watch?v=JxIZbV_XjAs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&ab_channel=TheCherno) from a youtuber called [The Cherno](https://www.youtube.com/@TheCherno). Without him, this engine could've been lot harder or even impossible due to time reasons and knowledge. In this series, a game engine is created also from scratch and worked week by week even to this day. The engine is called [Hazel](https://hazelengine.com/), and because of this explanation, it's seen that Soul Engine is a child of that engine in a certain way. It is also important to note that the base engine is not directly copied from Hazel, but rather manually written by the autor of this engine, following video per video, because of learning purposes.

The reference code stops after the base engine and all of the subsequent code is made all by the autor of the renderer, including all the techniques like Bloom or HDR, and the paint software. The paint tool was solely made by the autor without any kind of documentation, or tutorials, while the techniques were own implementations from the autor from the knowledge adquired from LearnOpenGL or other sources.

## Libraries used

The most important libraries used in this project are the following, that must be mentioned:

- [GLAD](https://glad.dav1d.de/)
- [GLFW](https://www.glfw.org/)
- [SPDLOG](https://github.com/gabime/spdlog)
- [ImGUI](https://github.com/ocornut/imgui)
- [GLM](https://github.com/g-truc/glm)
- [Assimp](https://www.assimp.org/)
- [Entt](https://github.com/skypjack/entt)
- [Premake](https://premake.github.io/docs/)

