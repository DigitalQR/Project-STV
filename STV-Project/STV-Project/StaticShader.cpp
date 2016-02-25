#include "StaticShader.h"
#include "Camera.h"

StaticShader::StaticShader() : Shader("static"),
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
	Camera::getMain()->buildViewMatrix();

	glUniformMatrix4fv(_UNIFORM_VIEW_MATRIX, 1, GL_FALSE, &Camera::getMain()->getViewMatrix()[0][0]);
	glUniformMatrix4fv(_UNIFORM_PROJECTION_MATRIX, 1, GL_FALSE, &Camera::getMain()->getProjectionMatrix()[0][0]);
}
