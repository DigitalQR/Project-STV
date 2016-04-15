#include "VoxelMesh.h"
#include "GameManager.h"
#include <exception>

VoxelMesh::VoxelMesh(Vectori& mesh_size, Vectori& mesh_offset) :
	MESH_SIZE(mesh_size), MESH_OFFSET(mesh_offset)
{
	//Reserve space for chunk
	_resources.resize(MESH_SIZE.x);
	for (int x = 0; x < MESH_SIZE.x; x++)
	{
		_resources[x].resize(MESH_SIZE.y);
		for (int y = 0; y < MESH_SIZE.y; y++)
			_resources[x][y].resize(MESH_SIZE.z);
		
	}

	for (int x = 0; x < MESH_SIZE.x; x++)
		for (int y = 0; y < MESH_SIZE.y; y++)
			for (int z = 0; z < MESH_SIZE.z; z++)
				_resources[x][y][z] = RES_AIR;
}

VoxelMesh::~VoxelMesh()
{
	if (_model_data != nullptr)
		delete _model_data;

	if (_element != nullptr)
	{
		delete _texture_model->model;
		delete _texture_model;
		delete _element;
	}
}

void VoxelMesh::SetResourceAt(int x, int y, int z, Resource block)
{
	_resources[x][y][z] = block;
	if (block != RES_AIR)
		full_gen_track++;
	else
		full_gen_track--;
}

void VoxelMesh::RebuildModel() 
{
	ConstructModel();

	Model* model = GameManager::getMain()->model_loader->CreateTerrainModel(_model_data->verts, _model_data->uvs, _model_data->normals, _model_data->texture_ids, _model_data->indices);
	
	if (!GetEmptyModelFlag())
		if (_texture_model != nullptr)
		{
			Model* old_model = _texture_model->model;
			if (old_model != nullptr)
				delete old_model;
		}
	

	if (_texture_model == nullptr)
		_texture_model = new TexturedModel(model, (GLuint)0);
	else
		_texture_model->model = model;

	if (_element == nullptr)
		_element = new Element3D(_texture_model);
	
}

void VoxelMesh::BuildModel()
{
	if (GetEmptyModelFlag())
		return;
	
	Model* model = GameManager::getMain()->model_loader->CreateTerrainModel(_model_data->verts, _model_data->uvs, _model_data->normals, _model_data->texture_ids, _model_data->indices);
	_texture_model = new TexturedModel(model, (GLuint)0);
	_element = new Element3D(_texture_model);

	delete _model_data;
	_model_data = nullptr;
}

void VoxelMesh::ConstructModel()
{
	if (_model_data != nullptr)
		_model_data->Clear();
	else
		_model_data = new ModelData();

	const Vectori offset = MESH_SIZE * MESH_OFFSET;
	array<array<array<Resource, 2>, 2>, 2> states;

	//Calcuate densities
	const float in_plane_diagonal = 2;
	const float max = 1 * 6 + in_plane_diagonal * 12;

	if(GameManager::getMain()->voxel_builder->IsSmoothMode())
		for (int x = 0; x <= MESH_SIZE.x; x++)
			for (int y = 0; y <= MESH_SIZE.y; y++)
				for (int z = 0; z <= MESH_SIZE.z; z++) 
				{
					int count = 0;
					Resource& resource = GetResourceAt(x, y, z);
					bool state = IsSolid(resource);
								

					if (IsSolid(GetResourceAt(x + 1, y, z)) == state)
						count++;
					if (IsSolid(GetResourceAt(x - 1, y, z)) == state)
						count++;
					if (IsSolid(GetResourceAt(x, y + 1, z)) == state)
						count++;
					if (IsSolid(GetResourceAt(x, y - 1, z)) == state)
						count++;
					if (IsSolid(GetResourceAt(x, y, z + 1)) == state)
						count++;
					if (IsSolid(GetResourceAt(x, y, z - 1)) == state)
						count++;


					if (IsSolid(GetResourceAt(x - 1, y, z - 1)) == state)
						count+= in_plane_diagonal;
					if (IsSolid(GetResourceAt(x + 1, y, z - 1)) == state)
						count+= in_plane_diagonal;
					if (IsSolid(GetResourceAt(x - 1, y, z + 1)) == state)
						count+= in_plane_diagonal;
					if (IsSolid(GetResourceAt(x + 1, y, z + 1)) == state)
						count+= in_plane_diagonal;

					if (IsSolid(GetResourceAt(x, y - 1, z - 1)) == state)
						count+= in_plane_diagonal;
					if (IsSolid(GetResourceAt(x, y + 1, z - 1)) == state)
						count+= in_plane_diagonal;
					if (IsSolid(GetResourceAt(x, y - 1, z + 1)) == state)
						count+= in_plane_diagonal;
					if (IsSolid(GetResourceAt(x, y + 1, z + 1)) == state)
						count+= in_plane_diagonal;

					if (IsSolid(GetResourceAt(x - 1, y - 1, z)) == state)
						count+= in_plane_diagonal;
					if (IsSolid(GetResourceAt(x + 1, y - 1, z)) == state)
						count+= in_plane_diagonal;
					if (IsSolid(GetResourceAt(x - 1, y + 1, z)) == state)
						count+= in_plane_diagonal;
					if (IsSolid(GetResourceAt(x + 1, y + 1, z)) == state)
						count+= in_plane_diagonal;
				

					float density = (count) / max;
					if (density == 0)
						density = 0.005f;

					resource.density = density;
				}

	for (int x = 0; x < MESH_SIZE.x; x++)
		for (int y = 0; y < MESH_SIZE.y; y++)
			for (int z = 0; z < MESH_SIZE.z; z++)
			{
				states[0][0][0] = GetResourceAt(x, y, z);
				states[1][0][0] = GetResourceAt(x + 1, y, z);
				states[0][1][0] = GetResourceAt(x, y + 1, z);
				states[1][1][0] = GetResourceAt(x + 1, y + 1, z);
				states[0][0][1] = GetResourceAt(x, y, z + 1);
				states[1][0][1] = GetResourceAt(x + 1, y, z + 1);
				states[0][1][1] = GetResourceAt(x, y + 1, z + 1);
				states[1][1][1] = GetResourceAt(x + 1, y + 1, z + 1);
				GameManager::getMain()->voxel_builder->BuildFaces(x + offset.x, y + offset.y, z + offset.z, states, *_model_data);
			}

	empty_model_flag = !(bool)(_model_data->verts.size());
}
