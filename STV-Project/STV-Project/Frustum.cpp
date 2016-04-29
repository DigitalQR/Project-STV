#include "Frustum.h"
#include "Camera.h"

#define PI 3.141592f
#define deg_rad(y) y * 3.14159265359f / 180.0f

void Frustum::GenerateFrustum() 
{
	const vec3 location = _camera.getLERPLocation();

	const float FoV = _camera.GetFoV();
	const float near_distance = _camera.GetNearClippingPlane();
	const float far_distance = _camera.GetFarClippingPlane();

	const float near_height = 2.0f * tan(deg_rad(FoV) / 2.0f) * near_distance;
	const float near_width = near_height * _camera.GetAspectRatio();
	
	const float far_height = 2.0f * tan(deg_rad(FoV) / 2.0f) * far_distance;
	const float far_width = far_height * _camera.GetAspectRatio();


	const vec3 forward = _camera.getForward();
	const vec3 left = _camera.getLeft();
	const vec3 up = _camera.getUp();

	//Corners of frustum
	const vec3 near_top_left = location + forward * near_distance + up * near_height / 2.0f + left * near_width / 2.0f;
	const vec3 near_top_right = location + forward * near_distance + up * near_height / 2.0f - left * near_width / 2.0f;
	const vec3 near_bottom_left = location + forward * near_distance - up * near_height / 2.0f + left * near_width / 2.0f;
	const vec3 near_bottom_right = location + forward * near_distance - up * near_height / 2.0f - left * near_width / 2.0f;

	const vec3 far_top_left = location + forward * far_distance + up * far_height / 2.0f + left * far_width / 2.0f;
	const vec3 far_top_right = location + forward * far_distance + up * far_height / 2.0f - left * far_width / 2.0f;
	const vec3 far_bottom_left = location + forward * far_distance - up * far_height / 2.0f + left * far_width / 2.0f;
	const vec3 far_bottom_right = location + forward * far_distance - up * far_height / 2.0f - left * far_width / 2.0f;


	_planes[0] = Plane(near_top_left, near_top_right, near_bottom_right); //Front
	_planes[1] = Plane(far_bottom_right, far_top_right, far_top_left); //Back

	_planes[2] = Plane(far_bottom_left, far_top_left, near_top_left); //Left
	_planes[3] = Plane(near_bottom_right, near_top_right, far_top_right); //Right

	_planes[4] = Plane(far_top_left, far_top_right, near_top_right); //Top
	_planes[5] = Plane(far_bottom_right, far_bottom_left, near_bottom_left); //Bottom
}


void Frustum::RunTests()
{
	for (element_pair pair : _tests)
	{
		bool inside = true;

		//Check if edges are infront of any plane
		//If they're behind even one, then they are not rendered
		for (Plane& plane : _planes)
		{
			bool all_behind_plane = true;
			
			for (unsigned int i = 0; i < 8; i++)
			{
				const vec3& point = pair.aabb.GetPoint(i);

				if (plane.SignedDistance(point) >= 0) 
				{
					all_behind_plane = false;
					break;
				}
			}


			if (all_behind_plane)
			{
				inside = false;
				break;
			}
		}

		pair.element->enabled = inside;
	}
}