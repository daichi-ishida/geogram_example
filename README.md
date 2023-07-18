**Table of Contents**

- [Requirements](#requirements)
  - [Minimum Requirements](#minimum-requirements)
- [File Tree](#file-tree)
- [How to build](#how-to-build)
  - [Windows](#windows)
    - [**Method 1**](#method-1)
    - [**Method 2**](#method-2)
    - [**Method 3**](#method-3)
  - [Linux](#linux)
    - [**gcc/g++**](#gccg)
    - [**clang++**](#clang)


## Requirements

### Minimum Requirements
-[CMake](https://cmake.org) 3.17+

-[C++](http://www.cplusplus.com)14+


## File Tree

The file tree below illustrate how you develop using this cmake project structure
```
.
├── .vscode                  ### Config for dev-env with vscode
├── cmake                    ### Use this if you want to define something with CMake
│   └── clang.cmake          ### is used for clang build
├── common                   ### is useful when you want to place header-only library
├── data                     ### is useful when you want to load data from your application
├── src
│   ├── AAA                  ### An application for "AAA"
│   │   ├── main.cpp         ### Codes for AAA
│   │   └── CMakeLists.txt   ### Edit this to link external libraries to AAA
│   └── BBB                  ### An application for "BBB"
│   │   ├── main.cpp         ### Codes for BBB
│   │   └── CMakeLists.txt
│   └── CMakeLists.txt       ### is used when there are dependencies between, for example, "AAA" and "BBB".
├── CMakePresets.json        ### CMake configuration presets (will be used to build this project with command)
├── CMakeLists.txt           ### Global setting for all applications
:
```

## How to build
First, clone this with **submodule**
```
git clone --recursive git@github.com:daichi-ishida/geogram_example.git
```
- [Windows](#windows)
- [Linux](#linux)

### Windows
- `vcpkg` is recommended to install/find library
- **(Optional)** if you make use of `preset` like `win-x64-release` in cmake, [Ninja](https://github.com/ninja-build/ninja) build system is required, which makes your building faster

#### **Method 1**
Using CMake GUI will be easiest choice because simply select the appropriate settings for your environment will automatically resolve Path for Visual C++ Compiler or toolchain.

#### **Method 2**
Open with **Visual Studio Code** and choose `win-x64-Release` preset and execute

#### **Method 3**
Other option is using **Visual Studio 2022 Developer Command Prompt** and run commands below
```sh
# in Visual Studio 2022 Developer Command Prompt
cd geogram_example
cmake --preset win-x64-release
cd build\win-x64-release
cmake --build .

cd ..\..\bin\release
point_check.exe
```

### Linux
- if you use `gcc/g++`, choose `gcc-linux-release` as CMake preset
- if you prefer to use `clang++`, be sure to have installed [Ninja](https://github.com/ninja-build/ninja) and choose `clang-linux` as CMake preset

#### **gcc/g++**
The preset name for `gcc/g++` is `gcc-linux-release`

run commands below
```sh
cd geogram_example

cmake --preset gcc-linux-release
cd build/gcc-linux-release
cmake --build .

./bin/release/point_check
```

#### **clang++**
The preset name for `clang++` is `clang-linux`

run commands below
```sh
cd geogram_example

cmake --preset clang-linux
cmake --build --preset release

./build/clang-linux/bin/release/point_check
```