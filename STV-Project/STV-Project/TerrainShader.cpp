#include "TerrainShader.h"
#include "Camera.h"
#include "GameManager.h"

TerrainShader::TerrainShader() : Shader("terrain"),
	_UNIFORM_VIEW_MATRIX(glGetUniformLocation(_program, "view_matrix")),
	_UNIFORM_PROJECTION_MATRIX(glGetUniformLocation(_program, "projection_matrix")),

	_UNIFORM_FOG_COLOUR(glGetUniformLocation(_program, "fog_colour")),
	_UNIFORM_FOG_DENSITY(glGetUniformLocation(_program, "fog_density")),
	_UNIFORM_FOG_GRADIENT(glGetUniformLocation(_program, "fog_gradient")),

	_UNIFORM_SUNLIGHT_COLOUR(glGetUniformLocation(_program, "sunlight_colour")),
	_UNIFORM_SUNLIGHT_DIRECTION(glGetUniformLocation(_program, "sunlight_direction")),

	_UNIFORM_LIGHT_LOCATION(glGetUniformLocation(_program, "light_location")),
	_UNIFORM_LIGHT_COLOUR(glGetUniformLocation(_program, "light_colour")),
	_UNIFORM_LIGHT_ATTENUATION(glGetUniformLocation(_program, "light_attenuation"))
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

	vec3& sunlight_colour = GameManager::getMain()->sunlight_colour;
	vec3& sunlight_direction = GameManager::getMain()->sunlight_direction;
	glUniform3f(_UNIFORM_SUNLIGHT_COLOUR, sunlight_colour.x, sunlight_colour.y, sunlight_colour.z);
	glUniform3f(_UNIFORM_SUNLIGHT_DIRECTION, sunlight_direction.x, sunlight_direction.y, sunlight_direction.z);

	Light& player_light = GameManager::getMain()->player_light;
	glUniform3f(_UNIFORM_LIGHT_LOCATION, player_light.location.x, player_light.location.y, player_light.location.z);
	glUniform3f(_UNIFORM_LIGHT_COLOUR, player_light.colour.x, player_light.colour.y, player_light.colour.z);
	glUniform3f(_UNIFORM_LIGHT_ATTENUATION, player_light.attenuation.x, player_light.attenuation.y, player_light.attenuation.z);

}

bool TerrainShader::PrepareModel(TexturedModel* textured_model)
{
	if (!Shader::PrepareModel(textured_model))
		return false;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textured_model->texture);
	return true;
}

void TerrainShader::PrepareInstance(Element3D* element)
{
	Shader::PrepareInstance(element);
}

