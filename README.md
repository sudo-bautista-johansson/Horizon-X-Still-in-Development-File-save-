# Horizon X

> Modular 2D Game Engine & Editor written in C++20

Horizon X is an experimental game engine focused on:

* modular architecture,
* performance,
* clean tooling,
* scalability,
* and developer-friendly workflows.

The project is currently in early development and aims to evolve into a lightweight engine/editor ecosystem for creating simple games and experimenting with engine architecture.

---

# Features

## Current

* OpenGL renderer
* GLFW window system
* Dear ImGui editor UI
* Modular architecture
* ECS-based design
* Custom editor panels

## Planned

* Scene system
* Asset browser
* Scripting support
* Audio system
* Physics
* Tilemaps
* Live reloading
* Debugging tools
* Plugin system

---

# Screenshots

> Screenshots coming soon.

---

# Technologies

| System       | Technology |
| ------------ | ---------- |
| Language     | C++20      |
| Build System | CMake      |
| Rendering    | OpenGL     |
| Windowing    | GLFW       |
| UI           | Dear ImGui |
| Math         | GLM        |
| ECS          | entt       |

---

# Project Structure

```text id="tbajz8"
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

# Philosophy

Horizon X focuses on:

* clean code,
* modular systems,
* learning-oriented development,
* maintainability,
* and iterative improvement.

The goal is not to compete with large AAA engines, but to build a solid and extensible engine architecture over time.

---

# Current Goals

* Build a stable 2D renderer
* Create a usable editor
* Develop reusable engine systems
* Learn advanced C++ architecture
* Experiment with tooling and rendering pipelines

---

# Building

## Requirements

* C++20 Compiler
* CMake 3.20+
* OpenGL
* Git

---

## Clone Repository

```bash id="22krna"
git clone https://github.com/yourname/HorizonX.git
cd HorizonX
```

---

## Generate Project Files

```bash id="2v7m6l"
cmake -B Build
```

---

## Build

```bash id="bbyu2y"
cmake --build Build
```

---

# Running

After building:

```bash id="14svig"
./Build/HorizonEditor
```

---

# Architecture

The engine is separated into independent modules:

* Renderer
* ECS
* Scene System
* Input
* Audio
* Physics
* Scripting
* Editor UI

This separation helps maintain scalability and code clarity as the project grows.

---

# Editor Panels

Planned editor panels include:

* Scene Hierarchy
* Inspector
* Console
* Asset Browser
* Viewport
* Toolbar

---

# Roadmap

See:

```text id="z3c6x7"
Docs/Roadmap.md
```

---

# Development Status

## Version

Horizon X v0.0.1 Alpha

## State

Early Development

---

# Future Vision

Long-term goals include:

* advanced tooling,
* plugin systems,
* scripting support,
* visual debugging,
* and eventually 3D experimentation.

---

# Contributing

Currently the project is solo-developed and experimental.

Contributions, ideas, and feedback may be welcomed in the future.

---

# License

This project is licensed under the MIT License.

---

# Author

Developed by Farei_0x

---

# Notes

This engine is built primarily for:

* learning,
* experimentation,
* architecture practice,
* and long-term engine development experience.
