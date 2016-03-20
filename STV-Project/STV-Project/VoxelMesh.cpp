#include "VoxelMesh.h"
#include "GameManager.h"


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
	ModelData model_data;
		
	const Vectori offset = MESH_SIZE * MESH_OFFSET;
	array<array<array<block_id, 2>, 2>, 2> states;

	for (int x = 0; x < MESH_SIZE.x; x++)
		for (int y = 0; y < MESH_SIZE.y; y++)
			for (int z = 0; z < MESH_SIZE.z; z++)
			{
				states[0][0][0] = GetBlockAt(x, y, z);
				states[1][0][0] = GetBlockAt(x+1, y, z);
				states[0][1][0] = GetBlockAt(x, y+1, z);
				states[1][1][0] = GetBlockAt(x+1, y+1, z);
				states[0][0][1] = GetBlockAt(x, y, z+1);
				states[1][0][1] = GetBlockAt(x+1, y, z+1);
				states[0][1][1] = GetBlockAt(x, y+1, z+1);
				states[1][1][1] = GetBlockAt(x+1, y+1, z+1);
				marching_cube_handler.BuildFaces(x + offset.x, y + offset.y, z + offset.z, states, model_data);
			}

	_model = GameManager::getMain()->model_loader->CreateModel(model_data.verts, model_data.uvs, model_data.normals, model_data.indices);
}


void MarchingCube::GenerateParts() 
{
	_model_parts[0][0][0][0][0][0][0][0] = ModelData();
	SetAllData(1, 1,0,0,0, 0,0,0,0);
	SetAllData(2, 1,1,0,0, 0,0,0,0);
	SetAllData(3, 1,0,0,0, 0,1,0,0);
	SetAllData(4, 1,0,0,0, 0,0,1,0);
	SetAllData(5, 0,1,1,1, 0,0,0,0);
	SetAllData(6, 1,1,0,0, 0,0,1,0);
	SetAllData(7, 0,1,0,0, 1,0,1,0);
	SetAllData(8, 1,1,1,1, 0,0,0,0);
	SetAllData(9, 1,0,1,1, 0,0,0,1);
	SetAllData(10, 1,0,1,0, 1,0,1,0);
	SetAllData(11, 1,0,1,1, 0,0,1,0);
	SetAllData(12, 0,1,1,1, 1,0,0,0);
	SetAllData(13, 1,0,1,0, 0,1,0,1);
	SetAllData(14, 0,1,1,1, 0,0,0,1);
}


void MarchingCube::SetAllData(unsigned int state_case, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7)
{
	ModelData model_data = ModelData(_case_vertices[state_case], _case_uvs[state_case], _case_normals[state_case], _case_indices[state_case]);
	
	for (int xr = 0; xr < 7; xr++)
		for (int yr = 0; yr < 7; yr++)
			for (int zr = 0; zr < 7; zr++)
				RotateAndSetData(model_data, xr, yr, zr, v0, v1, v2, v3, v4, v5, v6, v7);
}

void MarchingCube::SetData(ModelData& model_data, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7)
{
	_model_parts[v0][v1][v2][v3][v4][v5][v6][v7] = model_data;
}

void MarchingCube::RotateAndSetData(ModelData& model_data, unsigned int x, unsigned int y, unsigned int z, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7)
{
	bool t0 = v0;
	bool t1 = v1;
	bool t2 = v2;
	bool t3 = v3;
	bool t4 = v4;
	bool t5 = v5;
	bool t6 = v6;
	bool t7 = v7;
	ModelData new_model_data = model_data;
	new_model_data.Rotate(x, y, z, 0.5f, 0.5f, 0.5f);
		
	RotateIndices(x, y, z, t0, t1, t2, t3, t4, t5, t6, t7);
	SetData(new_model_data, t0, t1, t2, t3, t4, t5, t6, t7);
}

void MarchingCube::BuildFaces(int X, int Y, int Z, array<array<array<block_id, 2>, 2>, 2>& states, ModelData& model_data)
{
	unsigned int state_count = 0;
	bool flip_tri = false;
	array<array<array<bool, 2>, 2>, 2> state_case {0};

	for (int x = 0; x < 2; x++)
		for (int y = 0; y < 2; y++)
			for (int z = 0; z < 2; z++) 
			{
				if (states[x][y][z] != BLOCK_AIR) 
				{
					state_count++;
					state_case[x][y][z] = true;
				}
			}

	if(state_count > 4)
	{
		state_count = 8 - state_count;
		flip_tri = true;

		for (int x = 0; x < 2; x++)
			for (int y = 0; y < 2; y++)
				for (int z = 0; z < 2; z++)
					state_case[x][y][z] = !state_case[x][y][z];
	}

	if (state_count == 0)
		return;
				
	ModelData data = _model_parts
		[state_case[0][0][0]] [state_case[1][0][0]] [state_case[1][0][1]] [state_case[0][0][1]]
		[state_case[0][1][0]] [state_case[1][1][0]] [state_case[1][1][1]] [state_case[0][1][1]];

	data += Vectori(X,Y,Z);
	if (flip_tri) data.Flip();

	if (data.verts.size() == 0 || data.indices.size() == 0) {
		cout << state_case[0][0][0] << state_case[1][0][0] << state_case[1][0][1] << state_case[0][0][1] << 
			state_case[0][1][0] << state_case[1][1][0] << state_case[1][1][1] << state_case[0][1][1] << endl;
	}

	model_data += data;
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

