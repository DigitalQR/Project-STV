#pragma once
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\common.hpp"
#include "Model.h"
#include "Entity.h"
#include <iostream>

using namespace std;

class Shader
{
public:
	Shader(string name);
	void Init(string name, string vertex_source, string fragment_source);
	~Shader();
	string name;

	virtual void PrepareShader()
	{
		glUseProgram(_program);
	};
	virtual void PrepareModel(Model* model)
	{
		glBindVertexArray(model->getVAO());
	};
	virtual void PrepareInstance(Entity* entity) = 0;

protected:
	GLuint _program;

private:

	/**
		Compiles a shader of specified type from source.

		@params Type of shader, Source code of shader, Name of shader (For debug)
		@return shader ID
	*/
	static GLuint CreateShader(GLenum shader_type, string source, string shader_name);

	/**
	Reads a shader file from the "Shaders" folder.

	@params File name (including extention) of shader
	@return Shader source code
	*/
	static string ReadShader(string file_name);
};

