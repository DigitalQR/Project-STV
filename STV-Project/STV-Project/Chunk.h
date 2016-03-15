#pragma once
#include "MemoryListener.h"
#include "BlockGlossary.h"
#include "Terrain.h"
#include "Model.h"
#include "ModelLoader.h"

#include <array>
#include <vector>


using namespace std;


#define CHUNK_SIZE 16
#define CHUNK_MAX_HEIGHT 128
#define CHUNK_SURFACE_HEIGHT 32
#define SQRT_3 1.7320500807557

class Chunk
{
public:
	Chunk(Terrain* parent, int x, int z);
	virtual void BuildTerrain();
	~Chunk();

	block_id GetBlockAt(int x, int y, int z) 
	{
		x -= _X * CHUNK_SIZE;
		z -= _Z * CHUNK_SIZE;

		if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_MAX_HEIGHT || z >= CHUNK_SIZE)
			return BLOCK_UNKNOWN;

		return _blocks[x][y][z];
	}

	Model* GetTerrainModel() { return _model; }

protected:
	virtual void Generate();
	virtual void BuildModel();

	float CosineInterpolate(float a, float b, float blend)
	{
		float theta = blend * 3.14159f;
		float f = (1.0f - cosf(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;
	}

	int GetHeight(int x, int y);
	float GetRawNoise(int x, int y);
	float GetNoise(int x, int y, int smoothness);
	float GetSmoothNoise(int x, int y, int smoothness);

	void AddToPanel(int x, int y, int z, int x_point, int y_point, int z_point, 
		vector<float>& verts, vector<float>& uvs, vector<float>& normals, vector<unsigned int>& indices, unsigned int& index_track);
	

private:
	const int _X, _Z;

	array<array<array<block_id, CHUNK_SIZE>, CHUNK_MAX_HEIGHT>, CHUNK_SIZE> _blocks{ BLOCK_AIR };
	Model* _model;
	Terrain* _parent;


};
