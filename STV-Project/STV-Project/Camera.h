#pragma once
#include "Dependencies\glm\vec3.hpp"
#include "Dependencies\glm\mat4x4.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"

#define NEAR_CLIPPING_PLANE 0.1f
#define FAR_CLIPPING_PLANE 100.0f

using namespace glm;

class Camera
{
public:
	Camera();
	~Camera();

	void setFoV(float fov) 
	{
		if (fov > 0 && fov <= 300)
			_FoV = fov;
	}

	void setPosition(vec3& location)
	{
		_position = location;
	}
	vec3& getPosition()
	{
		return _position;
	}

	void setOffset(vec3& offset)
	{
		_offset = offset;
	}
	vec3& getOffset()
	{
		return _offset;
	}

	/**
	Methods for building the appropriate Camera matrices

	*/
	void buildViewMatrix();
	void rebuildProjectionMatrix();
	void rebuildProjectionMatrix(float aspect_ratio);

	mat4 getViewMatrix() { return _view_matrix; }
	mat4 getProjectionMatrix() { return _projection_matrix; }

	/**
	Generates a singleton

	@returns Camera singleton
	*/
	static Camera* getMain()
	{
		static Camera* MAIN = new Camera();
		return MAIN;
	}

private:
	vec3 _position;
	vec3 _offset;

	//Projection Matrix
	float _last_aspect_ratio = 1;
	float _FoV = 70;
	mat4 _projection_matrix;
	mat4 _view_matrix;
};

