#include "TerrainShader.h"
#include "Camera.h"

TerrainShader::TerrainShader() : Shader("terrain"),
_UNIFORM_MODEL_MATRIX(glGetUniformLocation(_program, "model_matrix")),
_UNIFORM_VIEW_MATRIX(glGetUniformLocation(_program, "view_matrix")),
_UNIFORM_PROJECTION_MATRIX(glGetUniformLocation(_program, "projection_matrix"))
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
	glUniformMatrix4fv(_UNIFORM_MODEL_MATRIX, 1, GL_FALSE, &element->getModelMatrix()[0][0]);
}

