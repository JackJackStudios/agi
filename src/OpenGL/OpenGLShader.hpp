#pragma once

#include "AGI/Shader.hpp"

#include <glm/glm.hpp>

namespace AGI {

	class OpenGLShader : public ShaderBase
	{
	public:
		OpenGLShader(const ShaderSources& shaderSources);
		virtual ~OpenGLShader();

		virtual void Bind() override;
		virtual void Unbind() override;
		
		virtual BufferLayout GetLayout() const override;
		virtual bool AttributeExists(const std::string& name) const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
	private:
		uint32_t m_RendererID;

		friend class Reflection;
	};

}