#include "TerrainTools.h"


void ModelData::Clear()
{
	verts.clear();
	uvs.clear();
	normals.clear();
	indices.clear();
	texture_ids.clear();
}


void ModelData::ReplaceTextureIDs(resource_id v0, resource_id v1, resource_id v2, resource_id v3, resource_id v4, resource_id v5, resource_id v6, resource_id v7)
{
	for (unsigned int& id : texture_ids)
	{
		switch (id)
		{
		case 0:
			id = (unsigned int)v0;
			break;
		case 1:
			id = (unsigned int)v1;
			break;
		case 2:
			id = (unsigned int)v2;
			break;
		case 3:
			id = (unsigned int)v3;
			break;
		case 4:
			id = (unsigned int)v4;
			break;
		case 5:
			id = (unsigned int)v5;
			break;
		case 6:
			id = (unsigned int)v6;
			break;
		case 7:
			id = (unsigned int)v7;
			break;
		};
	}
}

void ModelData::BuildNormals()
{
	//Reset normals
	normals.clear();
	normals.resize(verts.size(), 0);

	//Work out all normals for faces
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		unsigned int i0 = indices[i] * 3;
		unsigned int i1 = indices[i + 1] * 3;
		unsigned int i2 = indices[i + 2] * 3;

		vec3 a(verts[i0], verts[i0 + 1], verts[i0 + 2]);
		vec3 b(verts[i1], verts[i1 + 1], verts[i1 + 2]);
		vec3 c(verts[i2], verts[i2 + 1], verts[i2 + 2]);

		vec3 norm(0, 0, 0);
		norm = glm::cross(b - a, c - a);

		normals[i0] += norm.x;
		normals[i0 + 1] += norm.y;
		normals[i0 + 2] += norm.z;

		normals[i1] += norm.x;
		normals[i1 + 1] += norm.y;
		normals[i1 + 2] += norm.z;

		normals[i2] += norm.x;
		normals[i2 + 1] += norm.y;
		normals[i2 + 2] += norm.z;

	}

	//Normalize
	for (unsigned int i = 0; i < normals.size(); i += 3)
	{
		vec3 norm = glm::normalize(vec3(normals[i], normals[i + 1], normals[i + 2]));
		normals[i] = norm.x;
		normals[i + 1] = norm.y;
		normals[i + 2] = norm.z;

	}

}

void ModelData::BuildUVs()
{
	//Reset uvs
	uvs.clear();
	uvs.resize(verts.size() * 2 / 3, 0);

	unsigned int uv_i = 0;

	for (unsigned int i = 0; i < verts.size(); i += 3)
	{
		float x = verts[i];
		float y = verts[i + 1];
		float z = verts[i + 2];

		const float xn = normals[i];
		const float yn = normals[i + 1];
		const float zn = normals[i + 2];

		const float xna = abs(xn);
		const float yna = abs(yn);
		const float zna = abs(zn);


		if (xna >= yna && xna >= zna)
		{
			if (sign(xn) > 0)
			{
				uvs[uv_i] = z;
				uvs[uv_i + 1] = y;
			}
			else
			{
				uvs[uv_i] = y;
				uvs[uv_i + 1] = z;
			}
		}
		else if (yna >= xna && yna >= zna)
		{
			if (sign(yn) > 0)
			{
				uvs[uv_i] = x;
				uvs[uv_i + 1] = z;
			}
			else
			{
				uvs[uv_i] = x;
				uvs[uv_i + 1] = z;
			}
		}
		else if (zna >= xna && zna >= yna)
		{
			if (sign(zn) > 0)
			{
				uvs[uv_i] = y;
				uvs[uv_i + 1] = x;
			}
			else
			{
				uvs[uv_i] = x;
				uvs[uv_i + 1] = y;
			}
		}
		uv_i += 2;
	}
}

void CheckRatio(float& ratio)
{
	const float bound = 0.6;
	if (ratio < -1.0f + bound)
		ratio = -1.0f + bound;
	else if (ratio > 1.0f - bound)
		ratio = 1.0f - bound;
}

float InterpolateRatios(float ratio0, float ratio1, float p)
{
	CheckRatio(ratio0);
	CheckRatio(ratio1);
	return ratio0 * (1.0f - p) + ratio1 * (p);
}

void ModelData::Smooth(Resource v0, Resource v1, Resource v2, Resource v3, Resource v4, Resource v5, Resource v6, Resource v7)
{
	for (int i = 0; i < verts.size(); i += 3)
	{
		float& x = verts[i];
		float& y = verts[i + 1];
		float& z = verts[i + 2];

		//front
		if (z == 0)
		{
			if (x == 0.5f) {
				float x_ratio0 = v0.density - v1.density;
				float x_ratio1 = v4.density - v5.density;
				x += InterpolateRatios(x_ratio0, x_ratio1, y);
			}
			if (y == 0.5f) {
				float y_ratio0 = v0.density - v4.density;
				float y_ratio1 = v1.density - v5.density;
				y += InterpolateRatios(y_ratio0, y_ratio1, x);
			}
		}
		//Back
		else if (z == 1)
		{
			if (x == 0.5f) {
				float x_ratio0 = v3.density - v2.density;
				float x_ratio1 = v7.density - v6.density;
				x += InterpolateRatios(x_ratio0, x_ratio1, y);
			}
			if (y == 0.5f) {
				float y_ratio0 = v3.density - v7.density;
				float y_ratio1 = v2.density - v6.density;
				y += InterpolateRatios(y_ratio0, y_ratio1, x);
			}
		}
		//Left
		else if (x == 0)
		{
			if (z == 0.5f) {
				float z_ratio0 = v0.density - v3.density;
				float z_ratio1 = v4.density - v7.density;
				z += InterpolateRatios(z_ratio0, z_ratio1, y);
			}
			if (y == 0.5f) {
				float y_ratio0 = v0.density - v4.density;
				float y_ratio1 = v3.density - v7.density;
				y += InterpolateRatios(y_ratio0, y_ratio1, z);
			}
		}
		//Right
		else if (x == 1)
		{
			if (z == 0.5f) {
				float z_ratio0 = v1.density - v2.density;
				float z_ratio1 = v5.density - v6.density;
				z += InterpolateRatios(z_ratio0, z_ratio1, y);
			}
			if (y == 0.5f) {
				float y_ratio0 = v1.density - v5.density;
				float y_ratio1 = v2.density - v6.density;
				y += InterpolateRatios(y_ratio0, y_ratio1, z);
			}
		}
		//Bottom
		else if (y == 0)
		{
			if (x == 0.5f) {
				float x_ratio0 = v0.density - v1.density;
				float x_ratio1 = v3.density - v2.density;
				x += InterpolateRatios(x_ratio0, x_ratio1, z);
			}
			if (z == 0.5f) {
				float z_ratio0 = v0.density - v3.density;
				float z_ratio1 = v1.density - v2.density;
				z += InterpolateRatios(z_ratio0, z_ratio1, x);
			}
		}
		//Top
		else if (y == 1)
		{
			if (x == 0.5f) {
				float x_ratio0 = v4.density - v5.density;
				float x_ratio1 = v7.density - v6.density;
				x += InterpolateRatios(x_ratio0, x_ratio1, z);
			}
			if (z == 0.5f) {
				float z_ratio0 = v4.density - v7.density;
				float z_ratio1 = v5.density - v6.density;
				z += InterpolateRatios(z_ratio0, z_ratio1, x);
			}
		}
	}
}

void ModelData::Flip()
{
	reverse(indices.begin(), indices.end());

	for (unsigned int i = 0; i < texture_ids.size(); i++)
	{
		vec3 position;
		unsigned int id = texture_ids[i];

		switch (id)
		{
		case 0:
			position = vec3(0, 0, 0);
			break;
		case 1:
			position = vec3(1, 0, 0);
			break;
		case 2:
			position = vec3(1, 0, 1);
			break;
		case 3:
			position = vec3(0, 0, 1);
			break;
		case 4:
			position = vec3(0, 1, 0);
			break;
		case 5:
			position = vec3(1, 1, 0);
			break;
		case 6:
			position = vec3(1, 1, 1);
			break;
		case 7:
			position = vec3(0, 1, 1);
			break;
		};

		vec3 direction(
			verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]);

		direction -= position;
		position += normalize(direction);

		if (position.x == 0 && position.y == 0 && position.z == 0)
			texture_ids[i] = 0;
		else if (position.x == 1 && position.y == 0 && position.z == 0)
			texture_ids[i] = 1;
		else if (position.x == 1 && position.y == 0 && position.z == 1)
			texture_ids[i] = 2;
		else if (position.x == 0 && position.y == 0 && position.z == 1)
			texture_ids[i] = 3;
		else if (position.x == 0 && position.y == 1 && position.z == 0)
			texture_ids[i] = 4;
		else if (position.x == 1 && position.y == 1 && position.z == 0)
			texture_ids[i] = 5;
		else if (position.x == 1 && position.y == 1 && position.z == 1)
			texture_ids[i] = 6;
		else if (position.x == 0 && position.y == 1 && position.z == 1)
			texture_ids[i] = 7;
	}
}


void ModelData::Rotate(unsigned int x, unsigned int y, unsigned int z, float pivot_x, float pivot_y, float pivot_z)
{
	for (unsigned int i = 0; i < verts.size(); i += 3) {
		verts[i] -= pivot_x;
		verts[i + 1] -= pivot_y;
		verts[i + 2] -= pivot_z;
	}

	float temp_x;
	float temp_y;
	float temp_z;

	for (unsigned int i = 0; i < verts.size(); i += 3) {
		for (unsigned int t = x; t != 0; t--)
		{
			temp_x = verts[i];
			temp_y = verts[i + 1];
			temp_z = verts[i + 2];

			verts[i] = temp_x;
			verts[i + 1] = -temp_z;
			verts[i + 2] = temp_y;
		}
		for (unsigned int t = y; t != 0; t--)
		{
			temp_x = verts[i];
			temp_y = verts[i + 1];
			temp_z = verts[i + 2];

			verts[i] = temp_z;
			verts[i + 1] = temp_y;
			verts[i + 2] = -temp_x;
		}
		for (unsigned int t = z; t != 0; t--)
		{
			temp_x = verts[i];
			temp_y = verts[i + 1];
			temp_z = verts[i + 2];

			verts[i] = temp_y;
			verts[i + 1] = -temp_x;
			verts[i + 2] = temp_z;
		}
	}

	for (unsigned int i = 0; i < verts.size(); i += 3) {
		verts[i] += pivot_x;
		verts[i + 1] += pivot_y;
		verts[i + 2] += pivot_z;
	}


	for (unsigned int i = 0; i < texture_ids.size(); i++)
	{
		unsigned int x_track = x;
		unsigned int y_track = y;
		unsigned int z_track = z;

		while (x_track != 0)
		{
			switch (texture_ids[i])
			{
			case 0:
				texture_ids[i] = 4;
				break;
			case 1:
				texture_ids[i] = 5;
				break;
			case 2:
				texture_ids[i] = 1;
				break;
			case 3:
				texture_ids[i] = 0;
				break;
			case 4:
				texture_ids[i] = 7;
				break;
			case 5:
				texture_ids[i] = 6;
				break;
			case 6:
				texture_ids[i] = 2;
				break;
			case 7:
				texture_ids[i] = 3;
				break;
			};
			x_track--;
		}

		while (y_track != 0)
		{
			switch (texture_ids[i])
			{
			case 0:
				texture_ids[i] = 3;
				break;
			case 1:
				texture_ids[i] = 0;
				break;
			case 2:
				texture_ids[i] = 1;
				break;
			case 3:
				texture_ids[i] = 2;
				break;
			case 4:
				texture_ids[i] = 7;
				break;
			case 5:
				texture_ids[i] = 4;
				break;
			case 6:
				texture_ids[i] = 5;
				break;
			case 7:
				texture_ids[i] = 6;
				break;
			};
			y_track--;
		}

		while (z_track != 0)
		{
			switch (texture_ids[i])
			{
			case 0:
				texture_ids[i] = 4;
				break;
			case 1:
				texture_ids[i] = 0;
				break;
			case 2:
				texture_ids[i] = 3;
				break;
			case 3:
				texture_ids[i] = 7;
				break;
			case 4:
				texture_ids[i] = 5;
				break;
			case 5:
				texture_ids[i] = 1;
				break;
			case 6:
				texture_ids[i] = 2;
				break;
			case 7:
				texture_ids[i] = 6;
				break;
			};
			z_track--;
		}
	}
}