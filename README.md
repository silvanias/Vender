# Vender

Lumi is an OpenGL rasterizer. Lumi provides a modular architecture for scene setup, camera control, and rendering. It leverages modern C++ capabilities and provides a simple GUI using ImGui for real-time interaction.

Phong Lighting     |  Textured Box with Specular Map
:-------------------------:|:-------------------------:
<img width="396" alt="Phong Lighting" src="https://github.com/user-attachments/assets/0f5eed08-8fb3-4c72-93b0-65e77a7855cb" /> | <img width="396" alt="textured_box" src="https://github.com/user-attachments/assets/7a81bf4f-7789-4445-bc91-9660eda30fbc" />



This guide will walk you through the setup process for the `vender` OpenGL application using CMake and vcpkg for dependency management.

## Prerequisites

- CMake (version 3.15 or newer)
- A C++ compiler with C++20 support
- OpenGL drivers installed on your system

## Clone the Repository

Start by cloning the main project repository to your local machine:

```bash
git clone https://github.com/silvanias/Vender.git
cd vender
```

## Set up vcpkg

Create a `vcpkg` directory inside the project and set up vcpkg, a C++ library manager:

```bash
mkdir vcpkg
cd vcpkg
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh # On Windows use .\vcpkg\bootstrap-vcpkg.bat
```

Install the required libraries:

```bash
./vcpkg/vcpkg install glm
./vcpkg/vcpkg install glfw3
./vcpkg/vcpkg install glad
```

Again, on Windows, you would use `.\vcpkg\vcpkg` instead of `./vcpkg/vcpkg`.

## Clone Dear ImGui

Clone the Dear ImGui library inside the `lib` directory of your project:

```bash
mkdir lib
git clone https://github.com/ocornut/imgui.git lib/imgui
```

## Configure CMake

Create a `build` directory and navigate into it:

```bash
mkdir build
cd build
```

Run CMake to generate the build files:

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
```

## Build the Application

Within the `build` directory, compile the application:

```bash
cmake --build .
```

## Run the Application

After successfully building the application, execute it with the following command:

```bash
./vender
```

On Windows, you might need to navigate to the directory containing the generated executable and run `vender.exe`.

## Troubleshooting

If you encounter any issues during setup or compilation, ensure that:

- Your CMake and Git versions meet the minimum requirements.
- All the prerequisites are properly installed.
- The vcpkg toolchain file path in the CMake command is correct.

For additional help, refer to the documentation of the respective tools or libraries, or consider reaching out to their communities.
