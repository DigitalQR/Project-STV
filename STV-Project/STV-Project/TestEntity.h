#pragma once
#include "Entity.h"
#include "OBJLoader.h"

class TestEntity : public Entity
{
public:
	TestEntity()
	{
	}

	virtual void ReadyUp() 
	{
		map<string, Model*> models = OBJLoader::LoadOBJ("Res/Creatures/Player/T-Pose.obj");
		_texture = GameManager::getMain()->texture_loader->LoadPNG("Res/Creatures/Player/stv_charpal.png");

		for (map<string, Model*>::iterator it = models.begin(); it != models.end(); it++)
		{
			TexturedModel* tm = new TexturedModel(it->second, _texture);
			_textured_models.push_back(tm);
			this->AddElement(new Element3D(tm));
		}
	}


	~TestEntity()
	{
		for (TexturedModel* tm : _textured_models)
		{
			delete tm->model;
			delete tm;
		}
	}

protected:
	list<TexturedModel*> _textured_models;
	GLuint _texture;
};

#include <array>
#define TERRAIN_SIZE 128
#define MAX_HEIGHT 32

class TestEntity2 : public TestEntity 
{

public:
	void ReadyUp() override
	{
		array<array<float, TERRAIN_SIZE>, TERRAIN_SIZE> heights = getHeights();

		_texture = GameManager::getMain()->texture_loader->LoadPNG("Res/World/grass_tile.png");

		TexturedModel* tm = new TexturedModel(getTerrainModel(heights), _texture);
		this->AddElement(new Element3D(tm));
		_textured_models.push_back(tm);

		location = vec3(-TERRAIN_SIZE/2, -TERRAIN_SIZE/10, -TERRAIN_SIZE);
		pivot = vec3(TERRAIN_SIZE/2, 0, TERRAIN_SIZE/2);
	}

	array<array<float, TERRAIN_SIZE>, TERRAIN_SIZE> getHeights()
	{
		array<array<float, TERRAIN_SIZE>, TERRAIN_SIZE> heights{ 0 };

		for (int y = 0; y < TERRAIN_SIZE; y++)
			for (int x = 0; x < TERRAIN_SIZE; x++)
				heights[x][y] = floorf(GetSmoothNoise(x, y, 50));
			
			

		return heights;
	}

	float CosineInterpolate(float a, float b, float blend) 
	{
		float theta = blend * 3.14159f;
		float f = (1.0f - cosf(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;
	}

	float GetSmoothNoise(int x, int y, int smoothness) 
	{
		return GetNoise(x, y, smoothness) + GetNoise(x, y, smoothness / 2) / 4.0f + GetNoise(x, y, smoothness / 4) / 8.0f;
	}

	float GetNoise(int x, int y, int smoothness) 
	{
		int x_whole = floorf((float)(x) / (float)(smoothness));
		int y_whole = floorf((float)(y) / (float)(smoothness));
		float x_fract = (float)(x) / (float)(smoothness) - x_whole;
		float y_fract = (float)(y) / (float)(smoothness) - y_whole;

		float v1 = getRawNoise(x_whole, y_whole);
		float v2 = getRawNoise(x_whole + 1, y_whole);
		float v3 = getRawNoise(x_whole, y_whole + 1);
		float v4 = getRawNoise(x_whole + 1, y_whole + 1);

		float h1 = CosineInterpolate(v1, v2, x_fract);
		float h2 = CosineInterpolate(v3, v4, x_fract);

		return CosineInterpolate(h1, h2, y_fract);
	}

	float getRawNoise(int x, int y) 
	{
		srand(1873 + x* 14759 + y* 31249);
		rand(); //Throw away

		float value = rand() % MAX_HEIGHT - MAX_HEIGHT/2;
		return value;
	}

	Model* getTerrainModel(array<array<float, TERRAIN_SIZE>, TERRAIN_SIZE> heights)
	{
		vector<float> verts{};
		vector<float> uvs{};
		vector<float> normals{};
		vector<unsigned int> indices{};

		unsigned int start = 0;

		for (int y = 0; y < TERRAIN_SIZE; y++)
			for (int x = 0; x < TERRAIN_SIZE; x++)
			{
				const float height = heights[x][y];
				const float sqrt3 = sqrt(3);

				//Top Left
				verts.push_back(x - 0.5f);
				verts.push_back(height);
				verts.push_back(y + 0.5f);
				normals.push_back(-1.0f / sqrt3);
				normals.push_back(1.0f / sqrt3);
				normals.push_back(1.0f / sqrt3);
				uvs.push_back(0.0);
				uvs.push_back(0.0);


				//Top Right
				verts.push_back(x + 0.5f);
				verts.push_back(height);
				verts.push_back(y + 0.5f);
				normals.push_back(1.0f / sqrt3);
				normals.push_back(1.0f / sqrt3);
				normals.push_back(1.0f / sqrt3);
				uvs.push_back(1.0);
				uvs.push_back(0.0);

				//Bottom Left
				verts.push_back(x - 0.5f);
				verts.push_back(height);
				verts.push_back(y - 0.5f);
				normals.push_back(-1.0f / sqrt3);
				normals.push_back(1.0f / sqrt3);
				normals.push_back(-1.0f / sqrt3);
				uvs.push_back(0.0);
				uvs.push_back(1.0);

				//Bottom Right
				verts.push_back(x + 0.5f);
				verts.push_back(height);
				verts.push_back(y - 0.5f);
				normals.push_back(1.0f / sqrt3);
				normals.push_back(1.0f / sqrt3);
				normals.push_back(-1.0f / sqrt3);
				uvs.push_back(1.0);
				uvs.push_back(1.0);

				
				indices.push_back(start);
				indices.push_back(start + 1);
				indices.push_back(start + 2);

				indices.push_back(start + 2);
				indices.push_back(start + 1);
				indices.push_back(start + 3);

				start += 4;

				//Build wall (Right)
				if (x != TERRAIN_SIZE-1)
				{
					float next_height = heights[x+1][y];

					if (height != next_height);
					{
						//Top Left
						verts.push_back(x + 0.5f);
						verts.push_back(height);
						verts.push_back(y + 0.5f);
						normals.push_back(-1.0f / sqrt3);
						normals.push_back(-1.0f / sqrt3);
						normals.push_back(1.0f / sqrt3);
						uvs.push_back(0.0);
						uvs.push_back(0.0);


						//Top Right
						verts.push_back(x + 0.5f);
						verts.push_back(next_height);
						verts.push_back(y + 0.5f);
						normals.push_back(1.0f / sqrt3);
						normals.push_back(1.0f / sqrt3);
						normals.push_back(1.0f / sqrt3);
						uvs.push_back(1.0);
						uvs.push_back(0.0);

						//Bottom Left
						verts.push_back(x + 0.5f);
						verts.push_back(height);
						verts.push_back(y - 0.5f);
						normals.push_back(-1.0f / sqrt3);
						normals.push_back(-1.0f / sqrt3);
						normals.push_back(-1.0f / sqrt3);
						uvs.push_back(0.0);
						uvs.push_back(1.0);

						//Bottom Right
						verts.push_back(x + 0.5f);
						verts.push_back(next_height);
						verts.push_back(y - 0.5f);
						normals.push_back(1.0f / sqrt3);
						normals.push_back(1.0f / sqrt3);
						normals.push_back(-1.0f / sqrt3);
						uvs.push_back(1.0);
						uvs.push_back(1.0);


						indices.push_back(start);
						indices.push_back(start + 1);
						indices.push_back(start + 2);

						indices.push_back(start + 2);
						indices.push_back(start + 1);
						indices.push_back(start + 3);

						start += 4;
					}
				}

				//Build wall (Above)
				if (y != TERRAIN_SIZE-1)
				{
					float next_height = heights[x][y+1];

					if (height != next_height);
					{
						//Top Left
						verts.push_back(x - 0.5f);
						verts.push_back(next_height);
						verts.push_back(y + 0.5f);
						normals.push_back(-1.0f / sqrt3);
						normals.push_back(1.0f / sqrt3);
						normals.push_back(-1.0f / sqrt3);
						uvs.push_back(0.0);
						uvs.push_back(0.0);


						//Top Right
						verts.push_back(x + 0.5f);
						verts.push_back(next_height);
						verts.push_back(y + 0.5f);
						normals.push_back(1.0f / sqrt3);
						normals.push_back(1.0f / sqrt3);
						normals.push_back(-1.0f / sqrt3);
						uvs.push_back(1.0);
						uvs.push_back(0.0);

						//Bottom Left
						verts.push_back(x - 0.5f);
						verts.push_back(height);
						verts.push_back(y + 0.5f);
						normals.push_back(-1.0f / sqrt3);
						normals.push_back(-1.0f / sqrt3);
						normals.push_back(-1.0f / sqrt3);
						uvs.push_back(0.0);
						uvs.push_back(1.0);

						//Bottom Right
						verts.push_back(x + 0.5f);
						verts.push_back(height);
						verts.push_back(y + 0.5f);
						normals.push_back(1.0f / sqrt3);
						normals.push_back(-1.0f / sqrt3);
						normals.push_back(-1.0f / sqrt3);
						uvs.push_back(1.0);
						uvs.push_back(1.0);


						indices.push_back(start);
						indices.push_back(start + 1);
						indices.push_back(start + 2);

						indices.push_back(start + 2);
						indices.push_back(start + 1);
						indices.push_back(start + 3);

						start += 4;
					}
				}
			}

		return GameManager::getMain()->model_loader->CreateModel(verts, uvs, normals, indices);
	}
};