#pragma once
#include "MemoryListener.h"
#include "Terrain.h"
#include "VoxelMesh.h"

#include <array>
#include <vector>


using namespace std;


class Terrain;


#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 16

#define GEN_CAVE_SIZE 54
#define GEN_SURFACE_HEIGHT 32
#define GEN_SURFACE_START 32
#define GEN_MAX_HEIGHT 128

class Chunk : public VoxelMesh
{
public:
	Chunk(Terrain* parent, int x, int y, int z);
	virtual void BuildTerrain();

	block_id GetBlockAt(int x, int y, int z);

protected:
	virtual void Generate();

	float CosineInterpolate(float a, float b, float blend)
	{
		float theta = blend * 3.14159f;
		float f = (1.0f - cosf(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;
	}

	int GetHeight(int x, int y);
	int GetCaveChance(int x, int y, int z);
	float GetRawNoise(int x, int y, int z, float frequency);
	float GetNoise(int x, int y, int z, float frequency, int smoothness);
	float GetNoise(int x, int y, float frequency, int smoothness);
	float GetSmoothNoise(int x, int y, int z, float frequency, int smoothness);
	float GetSmoothNoise(int x, int y, float frequency, int smoothness);	

	void AddDebugPanel(int x, int y, int z, int x_point, int y_point, int z_point, vector<float>& verts, vector<float>& uvs, vector<float>& normals, vector<unsigned int>& indices, unsigned int& index_track);

private:
	Terrain* _parent;

};
