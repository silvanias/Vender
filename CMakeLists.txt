cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(vender)

find_package(glm CONFIG REQUIRED)
#find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)


set(APP_SOURCES
    main.cpp
    shader.h
    stb_image.h
    stb_load.cpp
    camera.h
)

add_executable(vender ${APP_SOURCES} ${IMGUI_SOURCES})
#target_include_directories(${PROJECT_NAME} PRIVATE
#    imgui
#    imgui/backends)


#target_link_libraries(vender
#    PRIVATE
#    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
#    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
#)

target_link_libraries(vender PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(vender PRIVATE glfw)
target_link_libraries(vender PRIVATE glad::glad)
target_link_libraries(vender PRIVATE glm::glm)