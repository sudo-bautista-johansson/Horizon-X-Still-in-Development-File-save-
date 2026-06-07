cmake_minimum_required(VERSION 3.20)
project(HorizonX LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Dependency management via FetchContent ───────────────────────────────
include(FetchContent)

# 1. GLFW
message(STATUS "Fetching GLFW...")
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
    GIT_SHALLOW TRUE
)
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(glfw)

# 2. GLAD (OpenGL loader)
message(STATUS "Configuring GLAD...")
add_library(glad STATIC 
    "Vendor/GLAD/src/glad.c"
    "Vendor/GLAD/include/glad/glad.h"
    "Vendor/GLAD/include/KHR/khrplatform.h"
)
target_include_directories(glad PUBLIC "Vendor/GLAD/include")
target_compile_definitions(glad PRIVATE GLAD_GLAPI_EXPORT)

# 3. GLM (Math library)
message(STATUS "Fetching GLM...")
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

# 4. EnTT (ECS)
message(STATUS "Fetching EnTT...")
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.12.2
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(entt)

# 5. ImGui (docking branch)
message(STATUS "Fetching ImGui (docking)...")
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
    GIT_SHALLOW TRUE
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    
    # We will build ImGui as a static library
    add_library(imgui STATIC
        "${imgui_SOURCE_DIR}/imgui.cpp"
        "${imgui_SOURCE_DIR}/imgui_draw.cpp"
        "${imgui_SOURCE_DIR}/imgui_tables.cpp"
        "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
    )
    
    target_include_directories(imgui PUBLIC 
        "${imgui_SOURCE_DIR}"
        "${imgui_SOURCE_DIR}/backends"
    )
    target_link_libraries(imgui PUBLIC glfw glad)
endif()

# ── Subdirectories ────────────────────────────────────────────────────────
add_subdirectory(HorizonEngine)
add_subdirectory(HorizonEditor)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT HorizonEditor)
add_subdirectory(Launcher)
