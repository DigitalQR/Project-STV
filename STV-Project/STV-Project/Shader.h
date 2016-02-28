#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\common.hpp"
#include "Model.h"
#include "Entity.h"
#include "RenderQueue.h"

#include <iostream>
#include <map>

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

	void AddForRender(Entity* entity) 
	{
		render_queue.AddToRenderQueue(entity);
	};
	RenderQueue render_queue;
	void Render();

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
