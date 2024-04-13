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
    main.cpp
    shader.cpp
    camera/camera.cpp
    gui/imgui/imgui_lifecycle.cpp
    gui/window/window.cpp
)
list(TRANSFORM APP_SOURCES PREPEND vender/)

set(LIB_SOURCES
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_glfw.cpp

    stb/stb_load.cpp
)
list(TRANSFORM LIB_SOURCES PREPEND lib/)

add_executable(vender ${APP_SOURCES} ${LIB_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    lib/imgui
    ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(vender PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(vender PRIVATE glfw)
target_link_libraries(vender PRIVATE glad::glad)
target_link_libraries(vender PRIVATE glm::glm)