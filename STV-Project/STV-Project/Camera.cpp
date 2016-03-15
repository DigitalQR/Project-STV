#include "Camera.h"
#include <iostream>

using namespace std;

Camera::Camera()
{
	_position = vec3(0, 70, 0);
	_offset = vec3(0, -10, -50);
}

Camera::~Camera()
{
	cout << "Deleting Camera." << endl;
}

void Camera::buildViewMatrix() 
{
	vec3 actual_position = _position - _offset;

	_view_matrix = glm::lookAt(
		actual_position, _position, vec3(0, 1, 0)
	); 
}

void Camera::rebuildProjectionMatrix()
{
	rebuildProjectionMatrix(_last_aspect_ratio);
}

void Camera::rebuildProjectionMatrix(float aspect_ratio)
{
	_projection_matrix = glm::perspective(
		_FoV, aspect_ratio, NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE
		);
	_last_aspect_ratio = aspect_ratio;
}
