#include "Camera.h"
#include <iostream>

using namespace std;

Camera::Camera()
{
}

Camera::~Camera()
{
	cout << "Deleting Camera." << endl;
}

void Camera::buildViewMatrix() 
{
	vec3 rotation = getLERPRotation();

	vec3 direction = vec3(
		sin(rotation.y),
		tan(rotation.x),
		cos(rotation.y)
		);

	vec3 lerp_location = getLERPLocation();

	_view_matrix = glm::lookAt(
		lerp_location, lerp_location + direction, vec3(0, 1, 0)
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
