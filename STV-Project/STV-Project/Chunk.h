#pragma once
#include "MemoryListener.h"
#include "Terrain.h"
#include "VoxelMesh.h"

#include <array>
#include <vector>


using namespace std;


class Terrain;


#define CHUNK_SIZE_X 25
#define CHUNK_SIZE_Y 25
#define CHUNK_SIZE_Z 25

#define GEN_CAVE_SIZE 54
#define GEN_SURFACE_HEIGHT 40
#define GEN_SURFACE_START 0
#define GEN_MAX_HEIGHT 128

class Chunk : public VoxelMesh
{
public:
	static Vectori GetChunkCoordsOf(int x, int y, int z)
	{
		Vectori chunk(0, 0, 0);

		chunk.x = floor((x*1.0f) / (CHUNK_SIZE_X*1.0f));
		chunk.y = floor((y*1.0f) / (CHUNK_SIZE_Y*1.0f));
		chunk.z = floor((z*1.0f) / (CHUNK_SIZE_Z*1.0f));

		return chunk;
	}


	Chunk(Terrain* parent, int x, int y, int z);
	void GeneratePoint(int height, int x, int y, int z);

	resource_id GetResourceAt(int x, int y, int z);

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
	int Get3DChance(int x, int y, int z, float frequency, int smoothness);
	float GetRawNoise(int x, int y, int z, float frequency);
	float GetNoise(int x, int y, int z, float frequency, int smoothness);
	float GetNoise(int x, int y, float frequency, int smoothness);
	float GetSmoothNoise(int x, int y, int z, float frequency, int smoothness);
	float GetSmoothNoise(int x, int y, float frequency, int smoothness);	

	void AddDebugPanel(int x, int y, int z, int x_point, int y_point, int z_point, vector<float>& verts, vector<float>& uvs, vector<float>& normals, vector<unsigned int>& indices, unsigned int& index_track);

private:
	Terrain* _parent;

};
