#include "Program.h"
#include <fstream>
#include <limits>
#include <glm/gtc/type_ptr.hpp>

// ============================================================================
// Shader Implementation
// ============================================================================

std::unique_ptr<Shader> Shader::GenerateFromSource(std::string name, GLenum shaderType, const std::string& source)
{
	const char* sourceChar = source.c_str();
	const GLint sourceSize = static_cast<GLint>(source.size());

	GLuint shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &sourceChar, &sourceSize);
	glCompileShader(shaderID);

	int compile_result = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_result);

	if (compile_result == GL_FALSE)
	{
		// printf는 포맷 스트링 공격에 취약할 수 있으므로 cout 사용 권장
		std::cerr << GetShaderErrorLog(name, shaderID, shaderType) << std::endl;
	}

	return std::make_unique<Shader>(name, shaderID);
}

std::unique_ptr<Shader> Shader::GenerateFromFilename(GLenum shaderType, const std::string& filepath)
{
	std::ifstream file(filepath, std::ios::in);
	std::string fileContents;

	if (!file.good())
	{
		Logging::LogError("Cannot find file at path: '{0}'", filepath);
		return nullptr; // 에러 발생 시 nullptr 반환 처리
	}

	file.seekg(0, std::ios::end);
	fileContents.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&fileContents[0], fileContents.size());
	file.close();

	return GenerateFromSource(filepath, shaderType, fileContents);
}

std::string Shader::GetShaderErrorLog(const std::string& name, GLuint shaderID, GLenum shaderType)
{
	int infoLogLength;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	std::string shaderTypeStr = "Unknown";
	if (shaderType == GL_VERTEX_SHADER) shaderTypeStr = "Vertex Shader";
	else if (shaderType == GL_FRAGMENT_SHADER) shaderTypeStr = "Fragment Shader";
	else if (shaderType == GL_COMPUTE_SHADER) shaderTypeStr = "Compute Shader";

	if (infoLogLength != 0)
	{
		std::vector<char> log(infoLogLength);
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, &log[0]);
		std::string errorLog(log.begin(), log.end());
		return "Error compiling " + name + " " + shaderTypeStr + ":\n" + errorLog;
	}

	return "Unknown Error compiling " + name + " " + shaderTypeStr;
}

Shader::Shader(std::string name, GLuint shaderID)
{
	ShaderID = shaderID;
	Name = name;
}

Shader::~Shader()
{
	glDeleteShader(ShaderID);
}


// ============================================================================
// Program Implementation
// ============================================================================

Program::Program(GLuint programID)
{
	ProgramID = programID;
	IsBound = false;
}

Program::~Program()
{
	glDeleteProgram(ProgramID);
}

void Program::BindProgram()
{
	glUseProgram(ProgramID);
	IsBound = true;
}

void Program::UnbindProgram()
{
	for (int i = 0; i < TextureCount; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glUseProgram(0);
	IsBound = false;
	TextureCount = 0;
}

GLuint Program::GetUniformID(const std::string& name)
{
	GLuint id = glGetUniformLocation(ProgramID, name.c_str());

	if (id == UINT_MAX) // -1 is UINT_MAX when cast to GLuint
		Logging::LogError("No uniform ID exists under the name '{0}'", name);

	return id;
}

GLuint Program::GetProgramID()
{
	return ProgramID;
}

void Program::SetBool(GLuint id, bool value)
{
	ValidateSetUniform(id);
	glUniform1i(id, value ? 1 : 0);
}

void Program::SetBool(const std::string& name, bool value)
{
	ValidateSetUniform(name);
	glUniform1i(GetUniformID(name), value ? 1 : 0);
}

void Program::SetFloat(GLuint id, float value)
{
	ValidateSetUniform(id);
	glUniform1f(id, value);
}

void Program::SetInt(GLuint id, int value)
{
	ValidateSetUniform(id);
	glUniform1i(id, value);
}

void Program::SetFloat(const std::string& name, float value)
{
	ValidateSetUniform(name);
	SetFloat(GetUniformID(name), value);
}

void Program::SetInt(const std::string& name, int value)
{
	ValidateSetUniform(name);
	SetInt(GetUniformID(name), value);
}

void Program::SetVector(GLuint id, glm::vec2 value)
{
	ValidateSetUniform(id);
	glUniform2f(id, value.x, value.y);
}

void Program::SetVector(GLuint id, glm::vec3 value)
{
	ValidateSetUniform(id);
	glUniform3f(id, value.x, value.y, value.z);
}

void Program::SetVector(GLuint id, glm::vec4 value)
{
	ValidateSetUniform(id);
	glUniform4f(id, value.x, value.y, value.z, value.w);
}

void Program::SetVector(const std::string& name, glm::vec2 value)
{
	ValidateSetUniform(name);
	SetVector(GetUniformID(name), value);
}

void Program::SetVector(const std::string& name, glm::vec3 value)
{
	ValidateSetUniform(name);
	SetVector(GetUniformID(name), value);
}

void Program::SetVector(const std::string& name, glm::vec4 value)
{
	ValidateSetUniform(name);
	SetVector(GetUniformID(name), value);
}

void Program::SetIVector(GLuint id, glm::ivec2 value)
{
	ValidateSetUniform(id);
	glUniform2i(id, value.x, value.y);
}

void Program::SetIVector(GLuint id, glm::ivec3 value)
{
	ValidateSetUniform(id);
	glUniform3i(id, value.x, value.y, value.z);
}

void Program::SetIVector(GLuint id, glm::ivec4 value)
{
	ValidateSetUniform(id);
	glUniform4i(id, value.x, value.y, value.z, value.w);
}

void Program::SetIVector(const std::string& name, glm::ivec2 value)
{
	ValidateSetUniform(name);
	SetIVector(GetUniformID(name), value);
}

void Program::SetIVector(const std::string& name, glm::ivec3 value)
{
	ValidateSetUniform(name);
	SetIVector(GetUniformID(name), value);
}

void Program::SetIVector(const std::string& name, glm::ivec4 value)
{
	ValidateSetUniform(name);
	SetIVector(GetUniformID(name), value);
}

void Program::SetMatrix(GLuint id, const glm::mat3x3& value)
{
	ValidateSetUniform(id);
	glUniformMatrix3fv(id, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetMatrix(GLuint id, const glm::mat4x4& value)
{
	ValidateSetUniform(id);
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetMatrix(const std::string& name, const glm::mat3x3& value)
{
	ValidateSetUniform(name);
	SetMatrix(GetUniformID(name), value);
}

void Program::SetMatrix(const std::string& name, const glm::mat4x4& value)
{
	ValidateSetUniform(name);
	SetMatrix(GetUniformID(name), value);
}

void Program::SetTexture(GLuint id, GLuint textureID)
{
	ValidateSetUniform(id);
	glActiveTexture(GL_TEXTURE0 + TextureCount);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(id, TextureCount);
	TextureCount++;
}

void Program::SetTexture(const std::string& name, GLuint textureID)
{
	ValidateSetUniform(name);
	glActiveTexture(GL_TEXTURE0 + TextureCount);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(GetUniformID(name), TextureCount);
	TextureCount++;
}

void Program::ValidateSetUniform(GLuint id)
{
	if (!IsBound)
		Logging::ThrowError("Trying to set Program variable while unbound (ID: {0})", id);
}

void Program::ValidateSetUniform(const std::string& name)
{
	if (!IsBound)
		Logging::ThrowError("Trying to set Program variable while unbound (Name: {0})", name);
}

std::unique_ptr<Program> Program::GenerateFromFileVsFs(const std::string& filepathVs, const std::string& filepathFs)
{
	auto vsShader = Shader::GenerateFromFilename(GL_VERTEX_SHADER, filepathVs);
	auto fsShader = Shader::GenerateFromFilename(GL_FRAGMENT_SHADER, filepathFs);

	std::vector<std::unique_ptr<Shader>> shaders;
	shaders.push_back(std::move(vsShader));
	shaders.push_back(std::move(fsShader));

	return GenerateProgram(std::move(shaders));
}

std::unique_ptr<Program> Program::GenerateFromFileCs(const std::string& filepathCs)
{
	auto csShader = Shader::GenerateFromFilename(GL_COMPUTE_SHADER, filepathCs);

	std::vector<std::unique_ptr<Shader>> shaders;
	shaders.push_back(std::move(csShader));

	return GenerateProgram(std::move(shaders));
}

std::unique_ptr<Program> Program::GenerateProgram(std::vector<std::unique_ptr<Shader>> shaders)
{
	GLuint programID = glCreateProgram();

	for (const auto& shader : shaders)
	{
		if (shader) glAttachShader(programID, shader->ShaderID);
	}

	glLinkProgram(programID);

	int link_result = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &link_result);

	if (link_result == GL_FALSE)
		Logging::ThrowError(GetProgramErrorLog(shaders, programID).c_str());

	return std::make_unique<Program>(programID);
}

std::string Program::GetProgramErrorLog(const std::vector<std::unique_ptr<Shader>>& shaders, GLuint programID)
{
	std::string name = "";
	if (!shaders.empty() && shaders[0]) name = shaders[0]->Name;

	for (size_t i = 1; i < shaders.size(); ++i)
		if (shaders[i]) name += ", " + shaders[i]->Name;

	int infoLogLength;
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength != 0)
	{
		std::vector<char> log(infoLogLength);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &log[0]);
		std::string errorLog(log.begin(), log.end());
		return "Program Error Linking Shaders: " + name + "\n" + errorLog;
	}

	return "Program Unknown Error Linking Shaders: " + name;
}