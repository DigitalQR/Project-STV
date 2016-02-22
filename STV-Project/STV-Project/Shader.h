#pragma once
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>

using namespace std;

class Shader
{
public:
	Shader(string name);
	~Shader();
	string name;
	GLuint getProgram() { return _program; }

private:
	GLuint _program;

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

