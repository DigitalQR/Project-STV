#include "StaticShader.h"
#include "Camera.h"

StaticShader::StaticShader() : Shader("static"),
	_UNIFORM_MODEL_MATRIX(glGetUniformLocation(_program, "model_matrix")),
	_UNIFORM_VIEW_MATRIX(glGetUniformLocation(_program, "view_matrix")),
	_UNIFORM_PROJECTION_MATRIX(glGetUniformLocation(_program, "projection_matrix"))
{
}


StaticShader::~StaticShader()
{
}

void StaticShader::PrepareShader()
{
	Shader::PrepareShader();
	glUniformMatrix4fv(_UNIFORM_VIEW_MATRIX, 1, GL_FALSE, &Camera::getMain()->getViewMatrix()[0][0]);
	glUniformMatrix4fv(_UNIFORM_PROJECTION_MATRIX, 1, GL_FALSE, &Camera::getMain()->getProjectionMatrix()[0][0]);
}

void StaticShader::PrepareModel(TexturedModel* textured_model)
{
	Shader::PrepareModel(textured_model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textured_model->texture);
}

void StaticShader::PrepareInstance(Element3D* element)
{
	Shader::PrepareInstance(element);
	glUniformMatrix4fv(_UNIFORM_MODEL_MATRIX, 1, GL_FALSE, &element->getModelMatrix()[0][0]);
}



