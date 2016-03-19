#include "VoxelMesh.h"
#include "GameManager.h"

#include <iostream>

VoxelMesh::VoxelMesh(Vectori& mesh_size, Vectori& mesh_offset) :
	MESH_SIZE(mesh_size), MESH_OFFSET(mesh_offset)
{
	//Reserve space for chunk
	_blocks.resize(MESH_SIZE.x);
	for (int x = 0; x < MESH_SIZE.x; x++)
	{
		_blocks[x].resize(MESH_SIZE.y);
		for (int y = 0; y < MESH_SIZE.y; y++)
			_blocks[x][y].resize(MESH_SIZE.z);
		
	}
}

VoxelMesh::~VoxelMesh()
{
	delete _model;
}

void VoxelMesh::SetBlockAt(int x, int y, int z, block_id block) 
{
	_blocks[x][y][z] = block;
}

void VoxelMesh::BuildModel()
{
	vector<float> verts{};
	vector<float> uvs{};
	vector<float> normals{};
	vector<unsigned int> indices{};

	unsigned int index_track = 0;
	
	const Vectori offset = MESH_SIZE * MESH_OFFSET;

	for (int x = 0; x < MESH_SIZE.x; x++)
		for (int y = 0; y < MESH_SIZE.y; y++)
			for (int z = 0; z < MESH_SIZE.z; z++)
			{
				if (GetBlockAt(x, y, z) != BLOCK_AIR) {

					if (GetBlockAt(x + 1, y, z) == BLOCK_AIR)
						AddToPanel(x + offset.x, y + offset.y, z + offset.z, 1, 0, 0, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x - 1, y, z) == BLOCK_AIR)
						AddToPanel(x + offset.x, y + offset.y, z + offset.z, -1, 0, 0, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x, y + 1, z) == BLOCK_AIR)
						AddToPanel(x + offset.x, y + offset.y, z + offset.z, 0, 1, 0, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x, y - 1, z) == BLOCK_AIR)
						AddToPanel(x + offset.x, y + offset.y, z + offset.z, 0, -1, 0, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x, y, z + 1) == BLOCK_AIR)
						AddToPanel(x + offset.x, y + offset.y, z + offset.z, 0, 0, 1, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x, y, z - 1) == BLOCK_AIR)
						AddToPanel(x + offset.x, y + offset.y, z + offset.z, 0, 0, -1, verts, uvs, normals, indices, index_track);

				}
			}


	_model = GameManager::getMain()->model_loader->CreateModel(verts, uvs, normals, indices);
}


void VoxelMesh::AddToPanel(int x, int y, int z, int x_point, int y_point, int z_point, vector<float>& verts, vector<float>& uvs, vector<float>& normals, vector<unsigned int>& indices, unsigned int& index_track)
{
	for (int a_sign = -1; a_sign <= 1; a_sign += 2)
		for (int b_sign = -1; b_sign <= 1; b_sign += 2)
		{
			if (x_point != 0)
			{
				verts.push_back(x + 0.5f * x_point);
				verts.push_back(y + 0.5f * a_sign);
				verts.push_back(z + 0.5f * b_sign);
				normals.push_back(1.0f / SQRT_3 * x_point);
				normals.push_back(1.0f / SQRT_3 * a_sign);
				normals.push_back(1.0f / SQRT_3 * b_sign);
			}
			if (y_point != 0)
			{
				verts.push_back(x + 0.5f * a_sign);
				verts.push_back(y + 0.5f * y_point);
				verts.push_back(z + 0.5f * b_sign);
				normals.push_back(1.0f / SQRT_3 * a_sign);
				normals.push_back(1.0f / SQRT_3 * y_point);
				normals.push_back(1.0f / SQRT_3 * b_sign);
			}
			if (z_point != 0)
			{
				verts.push_back(x + 0.5f * a_sign);
				verts.push_back(y + 0.5f * b_sign);
				verts.push_back(z + 0.5f * z_point);
				normals.push_back(1.0f / SQRT_3 * a_sign);
				normals.push_back(1.0f / SQRT_3 * b_sign);
				normals.push_back(1.0f / SQRT_3 * z_point);
			}

		}
	uvs.push_back(0.0);
	uvs.push_back(1.0);

	uvs.push_back(0.0);
	uvs.push_back(0.0);

	uvs.push_back(1.0);
	uvs.push_back(1.0);

	uvs.push_back(1.0);
	uvs.push_back(0.0);


	if (x_point > 0 || y_point < 0 || z_point > 0)
	{
		indices.push_back(index_track);
		indices.push_back(index_track + 3);
		indices.push_back(index_track + 1);

		indices.push_back(index_track + 2);
		indices.push_back(index_track + 3);
		indices.push_back(index_track);
	}
	else
	{
		indices.push_back(index_track + 1);
		indices.push_back(index_track + 3);
		indices.push_back(index_track);

		indices.push_back(index_track);
		indices.push_back(index_track + 3);
		indices.push_back(index_track + 2);
	}


	index_track += 4;
}

