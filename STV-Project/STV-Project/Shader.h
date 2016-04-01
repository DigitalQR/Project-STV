#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\common.hpp"

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

	virtual void PrepareModel(TexturedModel* textured_model)
	{
		glBindVertexArray(textured_model->model->getVAO());
		glPolygonMode(GL_FRONT_AND_BACK, textured_model->model->polygon_mode);
	};

	virtual void PrepareInstance(Element3D* e) {};

	void AddForRender(Element3D* e)
	{
		render_queue.AddToRenderQueue(e);
	};

	void RemoveFromRender(Element3D* e)
	{
		render_queue.RemoveFromRenderQueue(e);
	};

	void Render();

	void ClearRenderQueue() 
	{
		render_queue.queue.clear();
	}

protected:
	GLuint _program;
	RenderQueue render_queue;

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
