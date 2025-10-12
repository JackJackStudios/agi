# AGI 
[![CMake on multiple platforms](https://github.com/JackJackStudios/agi/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/JackJackStudios/agi/actions/workflows/cmake-multi-platform.yml)

AGI (**A**gnostic **G**raphics **I**nterface) is a library that implements a common abstraction layer over OpenGL and Vulkan along with some extra utilities for windowing and ImGui. 

The library is officially tested on Windows x64 and Unbuntu ARM64. See [github actions](https://github.com/JackJackStudios/agi/actions/workflows/cmake-multi-platform.yml) for current build status.

Key features:
- Easy to understand abstraction while still writing low-level code
- Direct interaction with the underlying API when necessary 
- Completely thread safe - no static/thread_local state
- Supportes Graphics and Compute pipelines 
- Extra utilies for hard concepts in computer graphics 

## Requirements 
* Windows or Linux (x64 or ARM64)
* CMake 3.15
* A C++ 20 compiler 

## Building AGI
To include AGI into a CMake project as a static library:

1. Add this repository to your workspace
2. Add a `add_subdirectory(agi)` call to your top-level CMakeLists.txt
4. Link `agi` with the necessary targets for the entire library:
5. `#include <AGI/agi.hpp>` to use library

To test and twick AGI examples:
```console
git clone https://github.com/JackJackStudios/agi
cd agi
mkdir build && cd build
cmake ..
```

## Using AGI in your Application
To use **AGI** in your application, simply include the main header:
```cpp
#include <AGI/agi.hpp>
```

### Initialization Overview
1.	Create a AGI::Window — this internally uses GLFW to create the OS window.
2.	Create a AGI::RenderContext — this sets up the rendering backend (e.g., OpenGL, Vulkan, or DirectX).
3.	Call Init() on the RenderContext — this performs thread-specific initialization.
Note: All subsequent calls to the Window or RenderContext must occur on the same thread that called Init().

If you don’t provide a custom logging callback, AGI will automatically print messages to std::cout.

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

    AGI::WindowProps windowProps;
    windowProps.Width = { 720, 720 };
    windowProps.Title = EXECUTABLE_NAME;

    auto window = AGI::Window::Create(settings, windowProps);
    auto context = AGI::RenderContext::Create(window);

    context->Init();
    
    while (!window->ShouldClose())
    {
        context->SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
        context->BeginFrame();

        context->EndFrame();
        window->PollEvents();
    }

    context->Shutdown();
    delete context;
    
    return 0;
}
```
## License

AGI is licensed under the [MIT License](LICENSE).
