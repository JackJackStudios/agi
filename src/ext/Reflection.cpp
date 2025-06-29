#include "AGI/ext/Reflection.hpp"

#include "OpenGL/OpenGLShader.hpp"

#include <glad/glad.h>

namespace AGI {

    namespace Utils {

        ShaderDataType OpenGLShaderTypeToAgiShaderType(GLenum type)
        {
            switch (type)
            {
            case GL_FLOAT:          return ShaderDataType::Float;
            case GL_FLOAT_VEC2:     return ShaderDataType::Float2;
            case GL_FLOAT_VEC3:     return ShaderDataType::Float3;
            case GL_FLOAT_VEC4:     return ShaderDataType::Float4;
            case GL_FLOAT_MAT3:     return ShaderDataType::Mat3;
            case GL_FLOAT_MAT4:     return ShaderDataType::Mat4;
            case GL_INT:            return ShaderDataType::Int;
            case GL_INT_VEC2:       return ShaderDataType::Int2;
            case GL_INT_VEC3:       return ShaderDataType::Int3;
            case GL_INT_VEC4:       return ShaderDataType::Int4;
            case GL_BOOL:           return ShaderDataType::Bool;
            }

            AGI_VERIFY(false, "Unknown ShaderDataType");
            return (ShaderDataType)0;
        }

    }

	std::vector<Attribute> AGI::Reflection::GetAttributes(Shader& shader)
	{
        OpenGLShader* glShader = std::dynamic_pointer_cast<OpenGLShader>(shader).get();

        GLint maxNameLen = 0;
        GLint numAttribs = 0;
        glGetProgramiv(glShader->m_RendererID, GL_ACTIVE_ATTRIBUTES, &numAttribs);
        glGetProgramiv(glShader->m_RendererID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLen);
        
        std::vector<Attribute> attributes(numAttribs);
        std::vector<char> nameData(maxNameLen);
        
        for (int i = 0; i < numAttribs; ++i) 
        {
            GLint size = 0;
            GLenum type = 0;
            GLsizei length = 0;

            glGetActiveAttrib(glShader->m_RendererID, i, maxNameLen, &length, &size, &type, nameData.data());
            std::string name(nameData.data(), length);

            GLint location = glGetAttribLocation(glShader->m_RendererID, name.c_str());

            attributes[i].Location = location;
            attributes[i].Name = name;
            attributes[i].Type = Utils::OpenGLShaderTypeToAgiShaderType(type);
            attributes[i].Size = ShaderDataTypeSize(attributes[i].Type);
        }

        return attributes;
	}

}