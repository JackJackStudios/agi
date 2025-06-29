#pragma once

#include "agipch.hpp"

#include "AGI/Shader.hpp"

namespace AGI {

    struct Attribute
    {
        std::string Name;

        ShaderDataType Type;
        int32_t Size;
        int32_t Location;
    };

    class Reflection
    {
    public:
        static std::vector<Attribute> GetAttributes(Shader& shader);
    };

};