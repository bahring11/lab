#pragma once

// [수정] 필수 헤더 파일 추가
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view> // C++17 string_view
#include <vector>
#include <memory>
#include <format>      // C++20 format

#include <glm/glm.hpp>
#include <gl/gl3w.h>   // 프로젝트 설정에 따라 glad/glew로 변경 필요할 수 있음

class Logging
{
public:
	template<typename... Args>
	static void Log(const std::string_view& log, Args&&... args)
	{
		std::cout << "LOG: " << std::vformat(log, std::make_format_args(args...)) << std::endl;
	}

	template<typename... Args>
	static void LogError(const std::string_view& error, Args&&... args)
	{
		std::cout << "ERROR: " << std::vformat(error, std::make_format_args(args...)) << std::endl;
	}

	template<typename... Args>
	static void ThrowError(const std::string_view& error, Args&&... args)
	{
		std::string errMsg = std::vformat(error, std::make_format_args(args...));
		std::cout << "CRITICAL ERROR: " << errMsg << std::endl;
		throw std::runtime_error(errMsg);
	}
};

class Shader
{
public:
	static std::unique_ptr<Shader> GenerateFromSource(std::string programName, GLenum shaderType, const std::string& source);
	static std::unique_ptr<Shader> GenerateFromFilename(GLenum shaderType, const std::string& filepath);
	static std::string GetShaderErrorLog(const std::string& programName, GLuint shaderID, GLenum shaderType);

	GLuint ShaderID = 0;
	std::string Name = "";

	Shader(std::string name, GLuint shaderID);
	~Shader();
};

class Program
{
public:
	static std::unique_ptr<Program> GenerateFromFileVsFs(const std::string& filepathVs, const std::string& filepathFs);
	static std::unique_ptr<Program> GenerateFromFileCs(const std::string& filepathCs);
	static std::unique_ptr<Program> GenerateProgram(std::vector<std::unique_ptr<Shader>> shaders);

	Program(GLuint programID);
	~Program();

	void BindProgram();
	void UnbindProgram();

	GLuint GetUniformID(const std::string& name);
	GLuint GetProgramID();

	// [추가] MyGlWindow와의 호환성을 위해 추가된 헬퍼 함수
	GLuint getHandle() const { return ProgramID; }

	void SetBool(GLuint id, bool value);
	void SetBool(const std::string& name, bool value);

	void SetInt(GLuint id, int value);
	void SetInt(const std::string& name, int value);

	void SetFloat(GLuint id, float value);
	void SetFloat(const std::string& name, float value);

	void SetVector(GLuint id, glm::vec2 value);
	void SetVector(GLuint id, glm::vec3 value);
	void SetVector(GLuint id, glm::vec4 value);

	void SetVector(const std::string& name, glm::vec2 value);
	void SetVector(const std::string& name, glm::vec3 value);
	void SetVector(const std::string& name, glm::vec4 value);

	void SetIVector(GLuint id, glm::ivec2 value);
	void SetIVector(GLuint id, glm::ivec3 value);
	void SetIVector(GLuint id, glm::ivec4 value);

	void SetIVector(const std::string& name, glm::ivec2 value);
	void SetIVector(const std::string& name, glm::ivec3 value);
	void SetIVector(const std::string& name, glm::ivec4 value);

	void SetMatrix(GLuint id, const glm::mat3x3& value);
	void SetMatrix(GLuint id, const glm::mat4x4& value);

	void SetMatrix(const std::string& name, const glm::mat3x3& value);
	void SetMatrix(const std::string& name, const glm::mat4x4& value);

	void SetTexture(GLuint id, GLuint textureID);
	void SetTexture(const std::string& name, GLuint textureID);

private:
	static std::string GetProgramErrorLog(const std::vector<std::unique_ptr<Shader>>& shaders, GLuint programID);

	GLuint ProgramID = 0;
	bool IsBound = false;
	int TextureCount = 0;

	void ValidateSetUniform(GLuint id);
	void ValidateSetUniform(const std::string& name);
};