#pragma once
#include "MemoryListener.h"
#include "Terrain.h"
#include "VoxelMesh.h"

#include <array>
#include <vector>


using namespace std;


class Terrain;


#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_Z 16

#define GEN_CAVE_SIZE 60
#define GEN_SMOOTHNESS 60
#define GEN_SURFACE_HEIGHT 60
#define GEN_SNOW_HEIGHT 58
#define GEN_MOUNTAIN_HEIGHT 54
#define GEN_ORE_SIZE 50
#define GEN_SURFACE_START 0

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

	Resource& GetResourceAt(int x, int y, int z);

protected:
	virtual void Generate();

	float CosineInterpolate(float a, float b, float blend)
	{
		float theta = blend * 3.14159f;
		float f = (1.0f - cosf(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;
	}

	float Chunk::MwC_Rand(int x, int y, int z, int seed);
	float GetRawNoise(int x, int y, int z, float frequency);

	int GetHeight(int x, int y);
	int GetCaveChance(int x, int y, int z);
	int Get3DChance(int x, int y, int z, float frequency, int smoothness);

	float GetNoise(int x, int y, int z, float frequency, int smoothness);
	float GetNoise(int x, int y, float frequency, int smoothness);
	float GetSmoothNoise(int x, int y, int z, float frequency, int smoothness);
	float GetSmoothNoise(int x, int y, float frequency, int smoothness);	

	void AddDebugPanel(int x, int y, int z, int x_point, int y_point, int z_point, vector<float>& verts, vector<float>& uvs, vector<float>& normals, vector<unsigned int>& indices, unsigned int& index_track);


private:
	Terrain* _parent;

};
