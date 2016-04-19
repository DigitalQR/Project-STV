#include "HandShader.h"
#include "Camera.h"
#include "PlayerHand.h"
#include "GameManager.h"

HandShader::HandShader() : Shader("hand"),
_UNIFORM_MODEL_MATRIX(glGetUniformLocation(_program, "model_matrix")),
_UNIFORM_VIEW_MATRIX(glGetUniformLocation(_program, "view_matrix")),
_UNIFORM_PROJECTION_MATRIX(glGetUniformLocation(_program, "projection_matrix")),

_UNIFORM_SUNLIGHT_COLOUR(glGetUniformLocation(_program, "sunlight_colour")),
_UNIFORM_SUNLIGHT_DIRECTION(glGetUniformLocation(_program, "sunlight_direction")),

_UNIFORM_LIGHT_COLOUR(glGetUniformLocation(_program, "light_colour")),

_UNIFORM_ITEM_ID(glGetUniformLocation(_program, "item_id"))
{
}


HandShader::~HandShader()
{
}

void HandShader::PrepareShader()
{
	glClear(GL_DEPTH_BUFFER_BIT);

	Shader::PrepareShader();
	glUniformMatrix4fv(_UNIFORM_VIEW_MATRIX, 1, GL_FALSE, &Camera::getMain()->getStandardViewMatrix()[0][0]);
	glUniformMatrix4fv(_UNIFORM_PROJECTION_MATRIX, 1, GL_FALSE, &Camera::getMain()->getProjectionMatrix()[0][0]);

	vec3& sunlight_colour = GameManager::getMain()->sunlight_colour;
	vec3& sunlight_direction = GameManager::getMain()->sunlight_direction;
	glUniform3f(_UNIFORM_SUNLIGHT_COLOUR, sunlight_colour.x, sunlight_colour.y, sunlight_colour.z);
	glUniform3f(_UNIFORM_SUNLIGHT_DIRECTION, sunlight_direction.x, sunlight_direction.y, sunlight_direction.z);

	Light& player_light = GameManager::getMain()->player_light;
	glUniform3f(_UNIFORM_LIGHT_COLOUR, player_light.colour.x, player_light.colour.y, player_light.colour.z);

	glUniform1f(_UNIFORM_ITEM_ID, PlayerHand::GetCurrentItemID());
}

bool HandShader::PrepareModel(TexturedModel* textured_model)
{
	if (!Shader::PrepareModel(textured_model))
		return false;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textured_model->texture);
	return true;
}

void HandShader::PrepareInstance(Element3D* element)
{
	Shader::PrepareInstance(element);
	glUniformMatrix4fv(_UNIFORM_MODEL_MATRIX, 1, GL_FALSE, &element->getModelMatrix()[0][0]);
}



