# AGI 
[![CMake on multiple platforms](https://github.com/JackJackStudios/agi/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/JackJackStudios/agi/actions/workflows/cmake-multi-platform.yml)

AGI (**A**gnostic **G**raphics **I**nterface) is a library that implements a common abstraction layer over OpenGL and Vulkan along with some extra utilities for windowing and ImGui. 

The library is officially tested on Windows x64 and Unbuntu ARM64. See [github actions](https://github.com/JackJackStudios/agi/actions/workflows/cmake-multi-platform.yml) for current build status.

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
To include AGI into a CMake project as a static library:

1. Add this repository as a submodule or use FetchContent
2. Enable/disable extensions with options `AGI_EXTENSIONS`
3. Add a `add_subdirectory(agi)` call to your CMakeLists.txt
4. Add `agi` dependency to the necessary targets for the entire library:
5. To use extensions included in the repository enable `AGI_EXTENSIONS` in CMake and add dependencies:
	* `agi_imgui` for the ImGui extension

To build AGI with examples:

```console
git clone --recursive https://github.com/JackKnox/agi
cd agi
mkdir build && cd build
cmake ..
```
You can then run `cmake --build .` to build the repository including examples or use your prefered IDE to run the executables and tweek the source code.

## Using AGI in Applications
To use AGI in your application simply include `agi/agi.hpp`. To initialize the library you first must create a `AGI::Window` which uses GLFW, create the `RenderContext` which sets up the relevent AGI (e.g OpenGL, Vulkan).
Then you must call `Init()` on the `RenderContext`, this function calles all thread-specific initialisation, this is useful if you create a render thread. However any calles to either the `Window` or `RenderContext` must happen on the same thread after this call.

The example below setups up AGI with a custom message callback, if you don't do this AGI will output to `std::cout`.

```cpp
#include <iostream>
#include <agi/agi.hpp>

static void OnAGIMessage(std::string_view message, AGI::LogLevel level)
{
    switch (level)
    {
        case AGI::LogLevel::Trace:   std::cout << "[TRACE] " << message << std::endl; break;
        case AGI::LogLevel::Info:    std::cout << "[INFO] " << message << std::endl; break;
        case AGI::LogLevel::Warning: std::cout << "[WARN] " << message << std::endl; break;
        case AGI::LogLevel::Error:   std::cout << "[ERROR] " << message << std::endl; break;
    }
}

int main()
{
    AGI::Settings settings;
    settings.PreferedAPI = AGI::BestAPI();
    settings.MessageFunc = OnAGIMessage;
    settings.Blending = true;

    AGI::WindowProps windowProps;
    settings.Window.Width = 720;
    settings.Window.Height = 720;
    settings.Window.Title = EXECUTABLE_NAME;

    auto window = AGI::Window::Create(settings);
    auto context = AGI::RenderContext::Create(window);

    context->Init();
    
    while (!window->ShouldClose())
    {
        context->SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
        context->Clear();

        window->OnUpdate();
    }

    context->Shutdown();
    window.reset();
    return 0;
}
```
## License

AGI is licensed under the [MIT License](LICENSE).
