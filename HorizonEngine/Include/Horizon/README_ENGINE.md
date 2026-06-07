# Horizon Engine - Arquitectura base

Resumen rápido
- Lenguaje: C++
- Render backend: OpenGL (portátil, bien soportado y ya integrado con GLFW/GLAD)
- Dependencias: GLFW, GLAD, glm, imgui, entt
- Objetivo: multiplataforma (Windows, Linux, macOS)

Estructura propuesta
- HorizonEngine/
  - Include/Horizon/      -> headers públicos
  - Source/               -> implementación
  - ThirdParty/           -> libs no gestionadas por CMake (opcional)
- HorizonEditor/          -> editor/app
- Assets/                 -> recursos por defecto (shaders, texturas)
- CMakeLists.txt          -> top-level

Módulos mínimos implementados (base):
- Core: Application, Window, Layer, LayerStack, Events, Input
- Renderer: RenderCommand, Framebuffer, Shader, Texture, Camera (ortográfica), Renderer API (OpenGL)
- Scene/ECS: entidad básica con entt y componentes
- Editor: viewport, ImGui docking, panels básicos

Guía de uso rápido
1) Generar con CMake (visual studio 2026 x64):
   cmake -S . -B Build -G "Visual Studio 18 2026" -A x64
2) Compilar:
   cmake --build Build --config Debug --parallel
3) Ejecutar:
   Build\\HorizonEditor\\Debug\\HorizonEditor.exe

Contribuir y roadmap
- Añadir cámaras perspectiva
- Implementar renderer 3D con materiales, skybox y PBR
- Pipeline de assets (importadores FBX/GLTF)
- Integración multiplataforma CI

Licencia: MIT (por defecto). Si quieres otra, dime.
