cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32)
    set(VCPKG_TARGET_TRIPLET x64-windows-static-md)
endif()

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(vender)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

set(APP_SOURCES
    main.cpp
    glad/src/glad.c
)

add_executable(vender ${APP_SOURCES} ${IMGUI_SOURCES})

target_link_libraries(vender PRIVATE 
    ${OPENGL_LIBRARIES}
    glfw)

target_include_directories(vender PRIVATE 
    glad/include
)
