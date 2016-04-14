#pragma once
#include "MemoryListener.h"
#include "BlockGlossary.h"
#include "TexturedModel.h"
#include "ModelLoader.h"
#include "TerrainTools.h"
#include "Element3D.h"

#include <array>
#include <vector>
using namespace std;


class VoxelMesh
{
public:
	const Vectori MESH_SIZE;
	const Vectori MESH_OFFSET;

	VoxelMesh(Vectori& mesh_size, Vectori& mesh_offset);
	~VoxelMesh();

	virtual void BuildModel();
	virtual void RebuildModel();
	virtual void ConstructModel();

	virtual void SetResourceAt(int x, int y, int z, Resource block);

	virtual Resource& GetResourceAt(int x, int y, int z)
	{
		return _resources[x][y][z];
	}

	Element3D* GetElement() { return _element; };

	bool GetGenFlag() 
	{
		return full_gen_track == 0 || full_gen_track == MESH_SIZE.x * MESH_SIZE.y * MESH_SIZE.z;
	}

	bool GetEmptyModelFlag() 
	{
		return empty_model_flag;
	}

protected:
	unsigned int full_gen_track = 0;
	bool empty_model_flag = false;

private:
	ModelData* _model_data = nullptr;
	TexturedModel* _texture_model = nullptr;
	Element3D* _element = nullptr;
	vector<vector<vector<Resource>>> _resources;
};
