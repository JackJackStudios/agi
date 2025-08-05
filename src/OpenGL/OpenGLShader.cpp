#include "agipch.hpp"
#include "OpenGLShader.hpp"

#include <fstream>
#include <array>

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace AGI {

	namespace Utils {

		static GLenum ShaderTypeToGLType(ShaderType type)
		{
			if (type == ShaderType::Vertex)   return GL_VERTEX_SHADER;
			if (type == ShaderType::Fragment) return GL_FRAGMENT_SHADER;

			AGI_VERIFY(false, "Unknown shader type '{}'", (int)type);
			return GL_NONE;
		}

		GLint GetLocation(GLenum id, const char* attr)
		{
			return glGetUniformLocation(id, attr);
		}

		GLuint Compile(GLenum type, const std::string& source)
		{
			GLuint shader = glCreateShader(type);
			const GLchar* src = source.c_str();
			glShaderSource(shader, 1, &src, nullptr);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (!isCompiled) 
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());
				glDeleteShader(shader);

				AGI_ERROR("{}", infoLog.data());
				AGI_VERIFY(false, "Shader compilation failure!");
			}

			return shader;
		}

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

	OpenGLShader::OpenGLShader(const ShaderSources& shaderSources)
	{
		m_RendererID = glCreateProgram();

		std::vector<GLuint> shaderIDs(shaderSources.size());
		int shaderIndex = 0;

		for (const auto& [type, source] : shaderSources)
		{
			GLuint shader = Utils::Compile(Utils::ShaderTypeToGLType(type), source);
			glAttachShader(m_RendererID, shader);
			shaderIDs[shaderIndex++] = shader;
		}

		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);

		if (!isLinked)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, infoLog.data());

			AGI_ERROR("{0}", infoLog.data());
			AGI_VERIFY(false, "Shader link failure!");
			return;
		}

		for (GLuint id : shaderIDs)
		{
			glDetachShader(m_RendererID, id);
			glDeleteShader(id);
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	BufferLayout OpenGLShader::GetLayout() const
	{
		GLint maxNameLen = 0;
		GLint numAttribs = 0;
		glGetProgramiv(m_RendererID, GL_ACTIVE_ATTRIBUTES, &numAttribs);
		glGetProgramiv(m_RendererID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLen);

		BufferLayout layout;
		layout.Resize(numAttribs);

		for (int i = 0; i < numAttribs; ++i)
		{
			GLint size = 0;
			GLenum type = 0;
			GLsizei length = 0;

			std::vector<char> nameData(maxNameLen);
			glGetActiveAttrib(m_RendererID, i, maxNameLen, &length, &size, &type, nameData.data());
			GLint location = glGetAttribLocation(m_RendererID, nameData.data());

			layout[location].Name = std::string(nameData.data(), length);
			layout[location].Type = Utils::OpenGLShaderTypeToAgiShaderType(type);
			layout[location].Size = Utils::ShaderDataTypeSize(layout[location].Type);
		}

		layout.SetOffsets();
		return layout;
	}

	bool OpenGLShader::AttributeExists(const std::string& name) const
	{
		return Utils::GetLocation(m_RendererID, name.c_str()) == -1 ? false : true;
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		Bind();
		glUniform1i(Utils::GetLocation(m_RendererID, name.c_str()), value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		Bind();
		glUniform1iv(Utils::GetLocation(m_RendererID, name.c_str()), count, values);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		Bind();
		glUniform1f(Utils::GetLocation(m_RendererID, name.c_str()), value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		Bind();
		glUniform2f(Utils::GetLocation(m_RendererID, name.c_str()), value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		Bind();
		glUniform3f(Utils::GetLocation(m_RendererID, name.c_str()), value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		Bind();
		glUniform4f(Utils::GetLocation(m_RendererID, name.c_str()), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		Bind();
		glUniformMatrix3fv(Utils::GetLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		Bind();
		glUniformMatrix4fv(Utils::GetLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

}