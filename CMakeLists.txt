cmake_minimum_required(VERSION 4.0.0)
project(Renderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

option(ONLY_LIBS "" ON)
add_subdirectory(third-party/glew)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# Needed because glfw doesn't work with wayland
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
add_subdirectory(third-party/glfw)

add_executable("RendererApp" 
    src/Camera.cpp
    src/Camera2D.cpp
    src/CubeModel.cpp
    src/main.cpp
    src/MatrixUtils.cpp
    src/Model.cpp
    src/Movable3D.cpp
    src/Movable2D.cpp
    src/Object3D.cpp
    src/Object2D.cpp
    src/Shader.cpp
    src/Vector.cpp
    src/Window.cpp
    src/DrawingQueue.cpp
    src/EmptyShaderParameter.cpp
    src/RectangleModel.cpp
    src/Texture.cpp
    src/ShaderTextureParameter.cpp
    src/ShaderParameterBundle.cpp
    third-party/stb/src/stb_image.cpp
    )
file(COPY ./shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories("RendererApp" PRIVATE include)
target_include_directories("RendererApp" PRIVATE third-party/stb/include)
target_link_libraries("RendererApp" glfw GL libglew_static)


