#pragma once
#include "MemoryListener.h"
#include "Transform.h"
#include "Dependencies\glm\vec3.hpp"
#include "Dependencies\glm\mat4x4.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"

#define NEAR_CLIPPING_PLANE 0.1f
#define FAR_CLIPPING_PLANE 10000.0f

using namespace glm;

class Camera : public Transform
{
public:
	Camera();
	~Camera();

	void setFoV(float fov) 
	{
		if (fov > 0 && fov <= 300)
			_FoV = fov;
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

	//Projection Matrix
	float _last_aspect_ratio = 1;
	float _FoV = 70;
	mat4 _projection_matrix;
	mat4 _view_matrix;
};

