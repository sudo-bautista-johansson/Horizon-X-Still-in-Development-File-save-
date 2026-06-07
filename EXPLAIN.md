# Explicación breve de archivos (.h/.cpp)

Este fichero resume, a nivel alto, el propósito de los archivos .h y .cpp principales del workspace para que puedas continuar el desarrollo.

---

## HorizonEditor (Editor)

- Source/EditorLayer.h / .cpp
  - Contiene la capa principal del editor. Gestiona la ventana dockspace, invoca los panels, controla la creación de escenas, el framebuffer del viewport, y coordina estados Play/Edit/Pause. Actualmente actúa como orquestador (debe delegar más a sistemas).

- Source/EditorCamera.h / .cpp
  - Cámara usada por el editor: entrada WASD, rotación con botón derecho del ratón, paneo con botón medio, zoom con rueda. Sincroniza su transform con una OrthographicCamera para integrarse con el renderer 2D.

- Source/EditorContext.h / .cpp
  - Estructura global ligera (g_EditorContext) que guarda el Ref<Scene> activo, la entidad seleccionada, ruta del proyecto y flag IsPlaying. Se usa para sincronizar los panels y sistemas sin que EditorLayer gestione todo.

### Panels/

- Source/Panels/ViewportPanel.h / .cpp
  - Panel que renderiza el contenido del framebuffer (ImGui::Image), mantiene tamaño/bounds, focus/hover y provee utilidades: ScreenToViewport() y PickEntityAt() (usa Framebuffer::ReadPixel para picking por ID).

- Source/Panels/SceneHierarchyPanel.h / .cpp
  - Lista de entidades de la escena; permite seleccionar y eliminar entidades. Al seleccionar actualiza g_EditorContext.SelectedEntity.

- Source/Panels/InspectorPanel.h / .cpp
  - (Interfaz) Muestra y permite editar componentes de la entidad seleccionada (Transform, SpriteRenderer, ScriptComponent). [Implementación parcial esperada en el proyecto].

- Source/Panels/AssetBrowserPanel.h / .cpp
  - Navegador de archivos/assets. Permite navegar carpetas, doble-clic para abrir scripts (.lua). Está conectado a ScriptManager/ScriptingSystem para abrir/crear scripts y notificar cambios.

- Source/Panels/ScriptEditorPanel.h / .cpp
  - Editor de texto básico para scripts: OpenFile(path), OnImGuiRender() con InputTextMultiline, Save(). Al guardar llama ScriptingSystem::ReloadScripts().

### Widgets/

- Source/Widgets/Toolbar.h / .cpp
  - Barra de reproducción con botones Play / Pause / Stop; expone SceneState para controlar el flujo editor/runtime.

- Source/Widgets/Gizmos.h / .cpp
  - Pequeña UI overlay dentro del viewport para editar Transform (Translate/Rotate/Scale). Usa la OrthographicCamera para referencia.

---

## HorizonEditor/Systems

- Source/Systems/ScriptManager.h / .cpp
  - Sistema del editor que centraliza la creación y apertura de scripts. Llama a ScriptingSystem para crear archivos y notifica callbacks (para que AssetBrowser o ScriptEditor reaccionen).

---

## HorizonEngine (Engine runtime)

- Include/Horizon/Renderer/Framebuffer.h / Source/Renderer/Framebuffer.cpp
  - Abstracción de framebuffer OpenGL. Crea color attachment y depth/stencil; se añadió un attachment entero (GL_R32I) para ID-based picking y la función ReadPixel(attachment, x, y) para leer IDs desde CPU.

- Include/Horizon/Renderer/Renderer2D.h / Source/Renderer/Renderer2D.cpp
  - Renderizador 2D por batches. Se añadió soporte para un parámetro opcional entityID en DrawQuad(). Además se creó un shader adicional (IDShader) y un pequeño path inmediato que escribe el ID entero al attachment 1 (requiere dibujar un quad con la misma transform para que el pixel del ID quede escrito).
  - Nota: el sistema actual hace flush del batch para la pasada ID y luego reanuda el batch. Es funcional pero básico; se puede optimizar/ajustar.

- Include/Horizon/Renderer/Shader.h / Source/Renderer/Shader.cpp
  - Wrapper para compilar/enlazar shaders; usado por Renderer2D y Scene.

- Include/Horizon/Scene/Scene.h / Source/Scene/Scene.cpp
  - Gestión de entidades (entt), componentes y renderizado de la escena. OnUpdateEditor() y OnUpdateRuntime() hacen las llamadas a Renderer2D para dibujar quads. Se modificó para pasar el entity ID a DrawQuad() y se añadió Scene::Clone() (copia simple de entidades y componentes) para play/test runtime.

- Include/Horizon/ECS/Entity.h
  - Wrapper sobre entt::entity con utilidades de Add/Get/Has/Remove component y conversión implícita.

- Include/Horizon/ECS/Components.h
  - Definición de componentes básicos: TagComponent, TransformComponent, SpriteRendererComponent, CameraComponent y se importó ScriptComponent.

- Include/Horizon/ECS/ScriptComponent.h
  - Componente que almacena la ruta del script asociado a la entidad.

- Include/Horizon/Assets/AssetManager.h / Source/Assets/AssetManager.cpp
  - Cache y utilidades básicas para assets (lectura de ficheros de texto, cola de carga de texturas). ScriptManager / ScriptingSystem interactúan con la capa de assets para registrar/leer archivos.

---

## HorizonEditor/Scripting

- Source/Scripting/ScriptingSystem.h / .cpp
  - API para crear y abrir scripts en disco (templating de archivos .lua), y gestionar un callback que el editor registra para abrir automáticamente el archivo en ScriptEditorPanel. Actualmente es un stub sencillo; puede integrar Lua runtime en adelante.

---

## Flujo recomendado (arquitectura objetivo)

1. Paneles (UI) emiten intenciones (crear/abrir asset, seleccionar entidad, play/stop).
2. Systems (ScriptManager, AssetManager, SceneManager) realizan la acción (crear archivo, registrar asset, clonar escena) y notifican a panels mediante callbacks o mediante EditorContext.
3. Panels actualizan su vista (AssetBrowser.Refresh(), ScriptEditor.Open(path), SceneHierarchy.SetSelectedEntity(...)).
4. Runtime (SceneManager) ejecuta la escena clonada; EditorContext.IsPlaying sirve para distinguir render/editor vs runtime.

---

Nota final
- Muchos puntos están implementados de forma básica/provisional para poder iterar rápido. Recomendación práctica: hacer una pasada de refactor para convertir EditorLayer en un orquestador ligero y mover la lógica real a Systems (EditorUIManager, SceneManager, AssetManager, ScriptManager). Esto facilitará el soporte de Projects/Templates en el futuro.

Si querés, genero un roadmap corto y paso a paso para convertir la arquitectura (módulos a implementar y puntos de integración). Dime si querés eso y lo creo.
