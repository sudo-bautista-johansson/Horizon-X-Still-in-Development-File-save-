cmake_minimum_required(VERSION 3.20)
project(HorizonEditor CXX)

file(GLOB_RECURSE SOURCES 
    "Source/*.cpp"
    "Source/*.h"
)

# Ensure new scripting sources are picked up (explicit add if glob misses)
file(GLOB_RECURSE SCRIPT_SOURCES "Source/Scripting/*.cpp" "Source/Scripting/*.h")
list(APPEND SOURCES ${SCRIPT_SOURCES})


add_executable(HorizonEditor ${SOURCES})

target_include_directories(HorizonEditor PRIVATE 
    "Source"
)

target_link_libraries(HorizonEditor PRIVATE 
    HorizonEngine
    imgui
    glfw
    glad
)
