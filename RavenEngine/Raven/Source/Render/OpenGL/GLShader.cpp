


#include "GLShader.h"
#include "Logger/Console.h"


#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>




using namespace Raven;




#define GLSHADER_DELETE_VALID(shader) { if (shader != 0) glDeleteShader(shader); }
#define GLSHADER_ATTACH_VALID(program, shader) { if (shader != 0) glAttachShader(program, shader); }








GLShader::GLShader()
	: id(0)
{

}


GLShader::~GLShader()
{
	if (id != 0)
	{
		glDeleteProgram(id);
	}
}


GLShader* GLShader::Create(const std::string& name)
{
	GLShader* shader = new GLShader();
	shader->name = name;

	return shader;
}


void GLShader::SetSource(EGLShaderStage stage, const std::string& src)
{
	GLShaderCode code;
	code.stage = stage;
	code.src = src;

	code.isFile = false;
	source[stage] = code;
}


void GLShader::SetSourceFile(EGLShaderStage stage, const std::string& src)
{
	GLShaderCode code;
	code.stage = stage;
	code.src = src;
	code.isFile = true;

	source[stage] = code;
}


void GLShader::AddExSource(const std::string& tag, EGLShaderStage stage, const std::string& src)
{
	GLShaderCode code;
	code.stage = stage;
	code.src = src;
	code.isFile = false;

	exSource[tag] = code;
}


void GLShader::AddExSourceFile(const std::string& tag, EGLShaderStage stage, const std::string& src)
{
	GLShaderCode code;
	code.stage = stage;
	code.src = src;
	code.isFile = true;

	exSource[tag] = code;
}


void GLShader::RemoveExSource(EGLShaderStage stage)
{
	source.erase(stage);
}


void GLShader::RemoveExSource(const std::string& tag)
{
	exSource.erase(tag);
}


void GLShader::AddPreprocessor(const std::string& def)
{
	preprocessor.insert(def);
}


void GLShader::RemovePreprocessor(const std::string& def)
{
	preprocessor.erase(def);
}


static std::string ToString(EGLShaderStage stage)
{
	switch (stage)
	{
	case Raven::EGLShaderStage::Vertex: return "Vertex";
	case Raven::EGLShaderStage::Geometry: return "Geometry";
	case Raven::EGLShaderStage::Fragment: return "Fragment";
	}

	return "NONE";
}



bool GLShader::Build()
{
	int prevID = id;

	// OpenGL Shader for each stag
	GLUINT glshaders[3] = { 0 };


	// Build Vertex Shader...
	glshaders[0] = BuildStage(EGLShaderStage::Vertex);
	if (!glshaders[0])
	{
		return false;
	}


	// Build Fragment Shader...
	glshaders[1] = BuildStage(EGLShaderStage::Fragment);

	if (!glshaders[1]) // Build Failed?
	{
		glDeleteShader(glshaders[0]);
		return false;
	}


	// Has Geometry?
	if ( source.count(EGLShaderStage::Geometry) )
	{
		glshaders[2] = BuildStage(EGLShaderStage::Geometry);

		if (!glshaders[2]) // Build Failed?
		{
			glDeleteShader(glshaders[0]);
			glDeleteShader(glshaders[1]);
			return false;
		}
	}


	// Program...
	id = glCreateProgram();
	glAttachShader(id, glshaders[0]); // Vert
	glAttachShader(id, glshaders[1]); // Frag
	GLSHADER_ATTACH_VALID(id, glshaders[2]); // Geom
	glLinkProgram(id); // Link...

	int result;
	glGetProgramiv(id, GL_LINK_STATUS, &result);

	// Link Success?
	if (result != GL_TRUE)
	{
		// Log Error...
		int len = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
		std::string log;
		log.resize(len+1);

		glGetProgramInfoLog(id, len, NULL, log.data());
		LOGE("GLShader({0}) - Failed To Link Program: \n{1}", name.c_str(), log.c_str());

		glDeleteProgram(id);
		id = prevID;
	}
	else
	{
		// Delete old program...
		if (prevID != 0)
		{
			glDeleteProgram(prevID);
		}
	}

	// Cleanup...
	glDeleteShader(glshaders[0]);
	glDeleteShader(glshaders[1]);
	GLSHADER_DELETE_VALID(glshaders[2]);

	return result == GL_TRUE;
}


GLUINT GLShader::BuildStage(EGLShaderStage stage)
{
	// 
	std::string src;
	LoadSource(stage, src);

	// Failed to load source?
	if (src.empty())
	{
		LOGE("GLShader({0}) - Failed to load {1} source.", name.c_str(), ToString(stage).c_str());
		return 0;
	}

	// Build...
	int shaderID = glCreateShader((GLENUM)stage);
	const char* srcData = src.data();
	glShaderSource(shaderID, 1, &srcData, NULL);
	glCompileShader(shaderID);

	int val = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &val);

	// Success?
	if (val == GL_TRUE)
		return shaderID;

	// Log Error...
	int len = 0;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &len);
	std::string log;
	log.resize(len+1);

	glGetShaderInfoLog(shaderID, len, NULL, log.data());
	LOGE("GLShader({0}) - Failed To Compile {1}: \n{2}", name.c_str(), ToString(stage).c_str(), log.c_str());

	glDeleteShader(shaderID);
	return 0;
}


bool GLShader::LoadSource(EGLShaderStage stage, std::string& loadedSrc)
{
	// Main Shader Stage Source...
	const GLShaderCode& code = source[stage];

	std::string mainSrc = code.isFile ? ReadSrcFile(code.src) : code.src;
	size_t vidx = mainSrc.find("#version ");
	vidx = mainSrc.find("\n", vidx + 1);

	//
	if (vidx > mainSrc.size())
	{
		LOGE("GLShader({0}) - #version not found in ({1}) source", name.c_str(), ToString(stage).c_str());
		return false;
	}

	// #verion
	loadedSrc = mainSrc.substr(0, vidx + 1);

	// #define
	for (const auto& pre : preprocessor)
	{
		loadedSrc += pre;
	}

	// Ex Source...
	loadedSrc += "\n\n";

	for (const auto& ex : exSource)
	{
		// code
		const auto& exsrc = ex.second;

		// Different Stage?
		if (exsrc.stage != stage)
			continue;
		
		std::string exloadedSrc = exsrc.isFile ? ReadSrcFile(exsrc.src) : exsrc.src;
		loadedSrc += exloadedSrc;
	}

	// Main Source
	loadedSrc += mainSrc.substr(vidx + 1);
	return !loadedSrc.empty();
}


std::string GLShader::ReadSrcFile(const std::string& file)
{
	std::ifstream fs;
	fs.open(file.c_str(), std::ios::in | std::ios::binary);

	// Failed?
	if (!fs.is_open())
	{
		LOGE("GLShader({0}) - Failed to load source file {1}", name.c_str(), file.c_str());
		return "";
	}


	char buffer[256];
	std::string src;


	while (fs.good())
	{
		fs.read(buffer, 250);
		src.append(buffer, fs.gcount());
	}

	return src;
}


void GLShader::Use()
{
	glUseProgram(id);
}


void GLShader::SetUniform(const std::string& name, const float& value)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform1f(loc, value);
}


void GLShader::SetUniform(const std::string& name, const int& value)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform1i(loc, value);
}


void GLShader::SetUniform(const std::string& name, const glm::vec2& value)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform2f(loc, value.x, value.y);
}


void GLShader::SetUniform(const std::string& name, const glm::vec3& value)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform3f(loc, value.x, value.y, value.z);
}


void GLShader::SetUniform(const std::string& name, const glm::vec4& value)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform4f(loc, value.x, value.y, value.z, value.w);
}


void GLShader::SetUniform(const std::string& name, const glm::mat4& value)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}


void GLShader::SetUniform(const std::string& name, const glm::mat3& value)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}


void GLShader::BindUniformBlock(const std::string& blockName, int binding)
{
	int blockIdx = glGetUniformBlockIndex(0, blockName.c_str());
	glUniformBlockBinding(id, blockIdx, binding);
}

