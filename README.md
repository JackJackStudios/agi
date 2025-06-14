# AGI
[![CMake on multiple platforms](https://github.com/JackJackStudios/agi/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/JackJackStudios/agi/actions/workflows/cmake-multi-platform.yml)

## Introduction
AGI (**A**gnostic **G**raphics **I**interface) is a library that implements a common abstraction layer over OpenGl (Vulkan comming soon!) along with some extra utilities for windowing and text rendering. 

The library is officially tested on Windows x64 and Unbuntu ARM64.

Key features:
- Easy to understand abstraction while still writing low-level code
- Direct interaction with the underlying API when necessary 
- Supportes Graphics and Compute pipelines 
- Extra utilies for hard concepts in computer graphics 

## Requirements 
* Windows or Linux (x64 or ARM64)
* CMake 3.15
* A C++ 20 compiler 

## Building AGI
AGI can be configured as a suite of test or example programs or a set of static libraries.

To include AGI into a CMake project as static libraries:

1. Add this repository as a submodule
2. Enable/disable AGI-ext with setting `AGI_EXTENSIONS`
3. Add a `add_subdirectory(agi)` call to your CMakeLists.txt
4. Add dependencies to the necessary targets: 
	* `agi` for the interface headers, common utilities, and validation
	* `agi_opengl` for the OpenGL abstraction
	* `agi_window` for the windowing extension 
	* `agi_imgui` for the ImGui extension

To build AGI with tests and examples:

```console
git clone --recursive https://github.com/JackKnox/agi
cd agi
mkdir build && cd build
cmake ..
```
You can then run `cmake --build .` to build the repository including tests and examples or use your prefered IDE to run the executables or tweek the source code.

## License

AGI is licensed under the [MIT License](LICENSE).
