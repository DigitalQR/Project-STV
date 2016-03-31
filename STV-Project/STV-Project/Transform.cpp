#include "Transform.h"
#include "GameManager.h"
#include "Dependencies\glm\gtx\transform.hpp"

Transform::Transform()
{
	Init(vec3(), vec3(), 1.0f);
}

Transform::Transform(vec3& location)
{
	Init(location, vec3(), 1.0f);
}

Transform::Transform(vec3& location, vec3& rotation)
{
	Init(location, rotation, 1.0f);
}

Transform::Transform(vec3& location, vec3& rotation, float scale)
{
	Init(location, rotation, scale);
}

Transform::~Transform()
{
}

void Transform::Init(vec3& loc, vec3& rot, float scl) 
{
	location = loc;
	_last_location = loc;

	rotation = rot;
	_last_rotation = rot;

	scale = scl;
	_last_scale = scl;
}

vec3 Transform::getLERPLocation()
{
	float LERP = GameManager::getMain()->GetLERPTime();
	return _last_location * LERP + location * (1.0f - LERP);
}

float LERPAngle(float a, float b) 
{
	float LERP = GameManager::getMain()->GetLERPTime();
	return tan(atan(a) * LERP + atan(b) * (1.0f - LERP));
}

vec3 Transform::getLERPRotation()
{
	return vec3(
		LERPAngle(_last_rotation.x, rotation.x),
		LERPAngle(_last_rotation.y, rotation.y),
		LERPAngle(_last_rotation.z, rotation.z)
		);
}

float Transform::getLERPScale()
{
	float LERP = GameManager::getMain()->GetLERPTime();
	return _last_scale * LERP + scale * (1.0f - LERP);
}

mat4& Transform::getModelMatrix() 
{	
	if (_parent != nullptr)
	{
		_model_matrix = _parent->getModelMatrix();
	}
	else 
	{
		_model_matrix = mat4(1.0);
	}

	vec3 location = getLERPLocation();
	vec3 rotation = getLERPRotation();
	float scale = getLERPScale();

	_model_matrix = glm::translate(_model_matrix, location);
	_model_matrix = glm::scale(_model_matrix, vec3(scale, scale, scale));

	_model_matrix = glm::translate(_model_matrix, pivot);
	_model_matrix = glm::rotate(_model_matrix, rotation.x, vec3(1, 0, 0));
	_model_matrix = glm::rotate(_model_matrix, rotation.y, vec3(0, 1, 0));
	_model_matrix = glm::rotate(_model_matrix, rotation.z, vec3(0, 0, 1));
	_model_matrix = glm::translate(_model_matrix, -pivot);

	return _model_matrix;
}

void Transform::TransformUpdate()
{
	_last_location = location;
	_last_rotation = rotation;
	_last_scale = scale;
}