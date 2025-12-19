#include "Shader.h"
Shader Shader::CreateShader(const char* VERTEX_SHADER_PATH, const char* FRAGMENT_SHADER_PATH) {

	ShaderLog.LoggerName = "SHADER";
	ShaderLog.defaultColour = DBG_MAGENTA;
	ShaderLog.CLog("Attempting to create new shader program from shader paths:");

	std::string s = (ShaderFolderName + (std::string)VERTEX_SHADER_PATH);
	VERTEX_SHADER_PATH = s.c_str();
	std::string t = (ShaderFolderName + (std::string)FRAGMENT_SHADER_PATH);
	FRAGMENT_SHADER_PATH = t.c_str();

	ShaderLog.CLog("Vertex Shader: " + std::string(VERTEX_SHADER_PATH));
	ShaderLog.CLog("Fragment Shader: " + std::string(FRAGMENT_SHADER_PATH));

	std::string vShaderCode, fShaderCode;
	std::ifstream vShaderFile, fShaderFile;

	vShaderFile.exceptions(std::ifstream::badbit || std::ifstream::failbit);
	fShaderFile.exceptions(std::ifstream::badbit || std::ifstream::failbit);

	try {
		if (VERTEX_SHADER_PATH != nullptr) { // loaded correctly, continue
			vShaderFile.open(VERTEX_SHADER_PATH);
			std::stringstream vShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			vShaderFile.close();
			vShaderCode = vShaderStream.str();
		}
		if (FRAGMENT_SHADER_PATH != nullptr) {
			fShaderFile.open(FRAGMENT_SHADER_PATH);
			std::stringstream fShaderStream;
			fShaderStream << fShaderFile.rdbuf();
			fShaderFile.close();
			fShaderCode = fShaderStream.str();
		}
	}
	catch (std::ifstream::failure) {
		ShaderLog.Error("Fatal Error! Shader program creation failed! File not found.");
	}

	std::string vShaderSourceStr = vShaderCode;
	std::string fShaderSourceStr = fShaderCode;
	unsigned int vShader, fShader;

	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);


	const char* VSHADER_SOURCE;
	stripUnicode(vShaderSourceStr);
	VSHADER_SOURCE = vShaderSourceStr.c_str();
	glShaderSource(vShader, 1, &VSHADER_SOURCE, NULL);
	glCompileShader(vShader);
	ShaderLog.CLog("Trying to compile shader VERTEX_SHADER");
	checkCompileErrors(vShader, "VERTEX_SHADER");

	const char* FSHADER_SOURCE;
	stripUnicode(fShaderSourceStr);
	FSHADER_SOURCE = fShaderSourceStr.c_str();
	glShaderSource(fShader, 1, &FSHADER_SOURCE, NULL);
	glCompileShader(fShader);
	ShaderLog.CLog("Trying to compile shader FRAGMENT_SHADER");
	checkCompileErrors(fShader, "FRAGMENT_SHADER");

	shaderID = glCreateProgram();

	ShaderLog.CLog("ShaderID: " + std::to_string(shaderID));

	glAttachShader(shaderID, vShader);
	glAttachShader(shaderID, fShader);

	glLinkProgram(shaderID);

	checkCompileErrors(shaderID, "PROGRAM");

	glDeleteShader(vShader);
	glDeleteShader(fShader);
	return *this;
}
int Shader::CurrentID() { return _currentID; }

int Shader::_currentID;

void Shader::Use() {
	if (Shader::_currentID == shaderID)
		return;  //check if currently active shader is this shader.
	else 
		Shader::_currentID = shaderID;
	glUseProgram(shaderID);
}
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			ShaderLog.Error("SHADER COMPILATION ERROR of type: " + type);
			ShaderLog.Error(infoLog);
		}
		else
			ShaderLog.CLog("SHADER COMPILATION SUCCESS");
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			ShaderLog.Error("SHADER CREATION ERROR of type: " + type);
			ShaderLog.Error(infoLog);
		}
		else
			ShaderLog.CLog("SHADER CREATION SUCCESS");
	}
}