# AGI

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
2. Add a `add_subdirectory(agi)` call to your CMakeLists.txt
3. Add dependencies to the necessary targets: 
	* `AGI` for the interface headers, common utilities, and validation
	* `AGI_opengl` for the OpenGL abstraction
	* `AGI_vulkan` for the Vulkan abstraction
	* `nvrhi_window` for Vulkan (enabled when `AGI_USE_WINDOW` is `ON`)
	* `nvrhi_text` for Vulkan (enabled when `AGI_USE_TEXT` is `ON`)

## License

AGI is licensed under the [MIT License](LICENSE).