#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Shader.h"

class TerrainShader : public Shader
{
public:
	TerrainShader();
	~TerrainShader();

	void PrepareShader();
	bool PrepareModel(TexturedModel* textured_model);
	void PrepareInstance(Element3D* element);

private:
	const GLuint _UNIFORM_VIEW_MATRIX;
	const GLuint _UNIFORM_PROJECTION_MATRIX;

	const GLuint _UNIFORM_FOG_COLOUR;
	const GLuint _UNIFORM_FOG_DENSITY;
	const GLuint _UNIFORM_FOG_GRADIENT;

	const GLuint _UNIFORM_SUNLIGHT_COLOUR;
	const GLuint _UNIFORM_SUNLIGHT_DIRECTION;

	const GLuint _UNIFORM_LIGHT_LOCATION;
	const GLuint _UNIFORM_LIGHT_COLOUR;
	const GLuint _UNIFORM_LIGHT_ATTENUATION;
	
};

