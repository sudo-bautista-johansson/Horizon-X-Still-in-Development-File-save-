cmake_minimum_required(VERSION 3.20)
project(HorizonEngine CXX)

file(GLOB_RECURSE SOURCES 
    "Source/*.cpp"
    "Source/*.h"
)

add_library(HorizonEngine STATIC ${SOURCES})

target_include_directories(HorizonEngine PUBLIC 
    "Include"
)

target_sources(HorizonEngine PRIVATE
    Source/Physics/Physics.cpp
    Source/Assets/AssetManager.cpp
    Source/Renderer/Renderer2D.cpp
    Source/Renderer/PerspectiveCamera.cpp
    Source/Renderer/FreeCameraController.cpp
    Source/Renderer/OrbitalCameraController.cpp
    Source/Renderer/Texture.cpp
    Source/Renderer/stb_image.cpp
    Source/Assets/AsyncLoader.cpp
    Source/Renderer/Camera.cpp
)

target_link_libraries(HorizonEngine PUBLIC 
    glfw
    glad
    imgui
    glm::glm
    EnTT::EnTT
)
