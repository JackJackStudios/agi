#pragma once

#include <glm/glm.hpp>

namespace AGI {

	enum class ShaderType
	{
		None = 0, Vertex, Fragment
	};

	ShaderType StringToShaderType(const std::string& type);

	using ShaderSources = std::unordered_map<ShaderType, std::string>;

	class ShaderBase
	{
	public:
		virtual ~ShaderBase() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual bool AttributeExists(const std::string& name) const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
	};

	ShaderSources ProcessSource(const std::string& source);
	using Shader = std::shared_ptr<ShaderBase>;

}