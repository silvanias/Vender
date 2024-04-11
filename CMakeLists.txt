cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vender/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(vender)

find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)


set(APP_SOURCES
    vender/main.cpp
    vender/shader.h
    vender/stb_image.h
    vender/stb_load.cpp
    vender/camera/camera.h
    vender/camera/camera.cpp
    vender/material.h
    vender/models/objects/cube.h
    vender/models/objects/pyramid.h
    vender/models/lighting/light.h
)

set(IMGUI_SOURCES
    vender/imgui/imgui_demo.cpp
    vender/imgui/imgui_draw.cpp
    vender/imgui/imgui_tables.cpp
    vender/imgui/imgui_widgets.cpp
    vender/imgui/imgui.cpp
    vender/imgui/backends/imgui_impl_opengl3.cpp
    vender/imgui/backends/imgui_impl_glfw.cpp)

add_executable(vender ${APP_SOURCES} ${IMGUI_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
    vender/imgui
    vender/imgui/backends)

target_link_libraries(vender PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(vender PRIVATE glfw)
target_link_libraries(vender PRIVATE glad::glad)
target_link_libraries(vender PRIVATE glm::glm)