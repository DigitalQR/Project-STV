#include "TerrainShader.h"
#include "Camera.h"
#include "GameManager.h"

TerrainShader::TerrainShader() : Shader("terrain"),
_UNIFORM_VIEW_MATRIX(glGetUniformLocation(_program, "view_matrix")),
_UNIFORM_PROJECTION_MATRIX(glGetUniformLocation(_program, "projection_matrix")),
_UNIFORM_FOG_COLOUR(glGetUniformLocation(_program, "fog_colour")),
_UNIFORM_FOG_DENSITY(glGetUniformLocation(_program, "fog_density")),
_UNIFORM_FOG_GRADIENT(glGetUniformLocation(_program, "fog_gradient"))
{
}


TerrainShader::~TerrainShader()
{
}

void TerrainShader::PrepareShader()
{
	Shader::PrepareShader();
	glUniformMatrix4fv(_UNIFORM_VIEW_MATRIX, 1, GL_FALSE, &Camera::getMain()->getViewMatrix()[0][0]);
	glUniformMatrix4fv(_UNIFORM_PROJECTION_MATRIX, 1, GL_FALSE, &Camera::getMain()->getProjectionMatrix()[0][0]);

	vec3& fog_colour = GameManager::getMain()->fog_colour;
	glUniform3f(_UNIFORM_FOG_COLOUR, fog_colour.x, fog_colour.y, fog_colour.z);
	glUniform1f(_UNIFORM_FOG_DENSITY, GameManager::getMain()->fog_density);
	glUniform1f(_UNIFORM_FOG_GRADIENT, GameManager::getMain()->fog_gradient);
}

void TerrainShader::PrepareModel(TexturedModel* textured_model)
{
	Shader::PrepareModel(textured_model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textured_model->texture);
}

void TerrainShader::PrepareInstance(Element3D* element)
{
	Shader::PrepareInstance(element);
}

