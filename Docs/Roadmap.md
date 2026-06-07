# Horizon X — Roadmap

> Modular 2D Game Engine & Editor written in C++20

---

# Vision

Horizon X aims to be a lightweight and modular game engine focused on:

* simplicity,
* performance,
* developer tools,
* experimentation,
* long-term scalability.

The project will start as a simple 2D engine and progressively evolve into a more advanced engine/editor ecosystem.

---

# Main Goals

* Learn advanced C++ architecture
* Build a clean modular engine
* Create a custom editor
* Support simple game creation
* Develop reusable tooling
* Keep the engine lightweight and fast

---

# Core Technologies

| System             | Technology |
| ------------------ | ---------- |
| Language           | C++20      |
| Build System       | CMake      |
| Graphics           | OpenGL     |
| Windowing          | GLFW       |
| OpenGL Loader      | GLAD       |
| UI                 | Dear ImGui |
| Math               | GLM        |
| ECS                | entt       |
| Audio              | miniaudio  |
| Scripting (future) | Lua        |

---

# Current Architecture

```text
HorizonX/
│
├── HorizonEngine/
├── HorizonEditor/
├── Sandbox/
├── Vendor/
├── Docs/
└── Build/
```

---

# Development Philosophy

* Keep systems modular
* Avoid premature optimization
* Build small working features first
* Prioritize maintainability
* Learn through iteration
* Focus on tools before complexity

---

# Phase 1 — Core Engine Foundation

## Goals

Create the minimal engine runtime.

## Features

* Window creation
* OpenGL context
* Main game loop
* Input handling
* Delta time
* Logging system
* Basic event system

## Status

* [ ] Window System
* [ ] Renderer Initialization
* [ ] Input Manager
* [ ] Event Dispatcher
* [ ] Logger
* [ ] Application Layer

---

# Phase 2 — 2D Rendering

## Goals

Render sprites and simple scenes.

## Features

* Orthographic camera
* Texture loading
* Sprite renderer
* Basic shaders
* Batch rendering
* Transform system

## Status

* [ ] Camera2D
* [ ] Texture System
* [ ] Sprite Renderer
* [ ] Shader System
* [ ] Render Queue

---

# Phase 3 — ECS & Scene System

## Goals

Create scalable entity management.

## Features

* Entity system
* Components
* Systems
* Scene serialization
* Scene hierarchy

## Status

* [ ] Entity Wrapper
* [ ] Component Registry
* [ ] Scene Manager
* [ ] Serialization
* [ ] Scene Loading

---

# Phase 4 — Editor

## Goals

Create a usable editor interface.

## Features

* Docking UI
* Scene hierarchy
* Inspector panel
* Viewport
* Toolbar
* Console
* Asset browser

## Status

* [ ] Editor Layer
* [ ] Dockspace
* [ ] Hierarchy Panel
* [ ] Inspector Panel
* [ ] Viewport
* [ ] Toolbar
* [ ] Console Panel

---

# Phase 5 — Assets & File System

## Goals

Handle engine resources properly.

## Features

* Asset manager
* Resource caching
* File explorer
* Asset importing
* Metadata system

## Status

* [ ] Asset Database
* [ ] Importer
* [ ] File Utilities
* [ ] Resource Cache

---

# Phase 6 — Audio

## Goals

Add sound support.

## Features

* Audio playback
* Sound effects
* Music streaming
* Volume controls

## Status

* [ ] Audio Engine
* [ ] Sound Manager
* [ ] Music System

---

# Phase 7 — Physics

## Goals

Add simple 2D physics.

## Features

* Collision detection
* Rigidbody2D
* Gravity
* Trigger system

## Status

* [ ] Physics World
* [ ] Collision System
* [ ] Rigidbody2D
* [ ] Debug Rendering

---

# Phase 8 — Scripting

## Goals

Allow gameplay scripting.

## Features

* Lua integration
* Script components
* Live reload
* API bindings

## Status

* [ ] Lua Runtime
* [ ] Script Loader
* [ ] Entity Bindings
* [ ] Hot Reload

---

# Phase 9 — Polish & Tools

## Goals

Improve usability and debugging.

## Features

* Profiler
* Debug overlays
* Performance graphs
* Memory statistics
* Engine settings

## Status

* [ ] Profiler
* [ ] Performance Metrics
* [ ] Debug UI
* [ ] Config System

---

# Future Ideas

## Possible Features

* Tilemap editor
* Networking
* 3D renderer
* Particle system
* Animation system
* Shader editor
* Visual scripting
* Plugin SDK
* Marketplace
* Multiplayer sandbox

---

# Coding Standards

## Naming

### Classes

```cpp
Renderer2D
SceneManager
Texture
```

### Methods

```cpp
LoadTexture()
RenderScene()
CreateEntity()
```

### Variables

```cpp
m_Camera
m_Window
s_Instance
```

---

# Important Rules

* No giant files
* One system per module
* Separate UI from engine runtime
* Avoid global state
* Document major systems
* Keep dependencies isolated

---

# Current Priority

## Immediate Tasks

1. Create OpenGL window
2. Initialize ImGui
3. Render first triangle
4. Create application loop
5. Add toolbar system

---

# Long-Term Goal

Create a lightweight but professional engine/editor ecosystem focused on:

* simplicity,
* extensibility,
* learning,
* tooling,
* experimentation.

---

# Project Status

## Current Version

Horizon X v0.0.1 Alpha

## State

Early development

## Lead Developer

Farei_0x
