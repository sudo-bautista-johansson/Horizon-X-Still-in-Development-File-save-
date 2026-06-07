cmake_minimum_required(VERSION 3.20)
project(HorizonLauncher CXX)

file(GLOB_RECURSE SOURCES
	"Source/*.cpp"
	"Source/*.h"
)

add_executable(HorizonLauncher ${SOURCES})

target_include_directories(HorizonLauncher PRIVATE
	"Source"
)

target_link_libraries(HorizonLauncher PRIVATE
	HorizonEngine
	imgui
	glfw
	glad
)
