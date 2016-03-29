#pragma once
#include "MemoryListener.h"
#include "Terrain.h"
#include "VoxelMesh.h"

#include <array>
#include <vector>


using namespace std;


#define CHUNK_SIZE 16
#define CHUNK_MAX_HEIGHT 128
#define CHUNK_CAVE_SIZE 54
#define CHUNK_SURFACE_HEIGHT 32

class Chunk : public VoxelMesh
{
public:
	Chunk(Terrain* parent, int x, int z);
	virtual void BuildTerrain();
	~Chunk();

	block_id GetBlockAt(int x, int y, int z)
	{
		if (x < 0 || y < 0 || z < 0 || x >= MESH_SIZE.x || y >= MESH_SIZE.y || z >= MESH_SIZE.z) {
			if (y > GetHeight(x, z)) {
				return BLOCK_AIR;
			}
			float cave_chance = GetCaveChance(x, y, z);
			if (cave_chance <= CHUNK_CAVE_SIZE)
				return BLOCK_AIR;

			return BLOCK_UNKNOWN;
		}

		return VoxelMesh::GetBlockAt(x,y,z);
	}

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
