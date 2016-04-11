#include "ChunkLoader.h"
#include "Terrain.h"
#include "Chunk.h"
#include "Camera.h"

ChunkLoader::~ChunkLoader()
{
	for (Chunk* chunk : _generated_list)
		delete chunk;

}

bool ChunkLoader::LoadChunk(int x, int y, int z) 
{
	//Check chunk is not already active
	for (Vectori chunk : _active_chunk_coords)
		if (chunk.x == x && chunk.y == y && chunk.z == z)
			return false;

	AddChunk(GetChunk(x, y, z));
	_active_chunk_coords.push_back(Vectori(x, y, z));
	return true;
}

void ChunkLoader::loading()
{
	Vectori current_location(-1000000000, -1000000001, -100000043);
	uint task_flag;



	vector<Vectori> offsets = GenerateOffsets(view_distance);

	while (running)
	{
		vec3& player_location = Camera::getMain()->location;
		Vectori new_coords = Chunk::GetChunkCoordsOf(floor(player_location.x), floor(player_location.y), floor(player_location.z));

		if (current_location != new_coords) 
		{
			current_location = new_coords;
			task_flag = 0;
		}
		
		//Select chunks to load
		while (task_flag < offsets.size())
		{
			Vectori offset = offsets[task_flag];
			if (LoadChunk(
				new_coords.x + offset.x,
				new_coords.y + offset.y,
				new_coords.z + offset.z
				)) 
			{
				task_flag++;
				break;
			}

			task_flag++;
		}

		//After finished loading cleanup, if possible
		if (task_flag >= offsets.size()) 
		{

			task_flag++;
		}

	}

	active = false;
}

void ChunkLoader::AddChunk(Chunk* chunk)
{
	if (running)
	{
		chunk->ConstructModel();
		_terrain->AddChunk(chunk);
	}
}

Chunk* ChunkLoader::GetChunk(int x, int y, int z)
{
	//Search in chunk cache
	for (Chunk* chunk : _chunk_cache)
	{
		if (chunk != nullptr)
			if (chunk->MESH_OFFSET.x == x && chunk->MESH_OFFSET.y == y && chunk->MESH_OFFSET.z == z)
			{
				UpdateCache(chunk);
				return chunk;
			}
	}
	

	//Search all
	for (Chunk* chunk : _generated_list)
	{
		if (chunk != nullptr)
			if (chunk->MESH_OFFSET.x == x && chunk->MESH_OFFSET.y == y && chunk->MESH_OFFSET.z == z)
			{
				UpdateCache(chunk);
				return chunk;
			}
	}

	//If not found, generate
	Chunk* chunk = new Chunk(_terrain, x, y, z);
	_generated_list.push_back(chunk);

	return chunk;
}

void ChunkLoader::UpdateCache(Chunk* new_front)
{
	Chunk* previous_value = new_front;
	for (unsigned int i = 0; i < _chunk_cache.size(); i++)
	{
		Chunk* temp = _chunk_cache[i];
		_chunk_cache[i] = previous_value;
		previous_value = temp;
		if (previous_value == nullptr || previous_value == _chunk_cache[i])
			break;
	}
}

vector<Vectori> ChunkLoader::GenerateOffsets(int view_distance) 
{
	vector<Vectori> offsets;
	offsets.push_back(Vectori(0, 0, 0));

	for (int i = 1; i <= view_distance; i++)
	{
		//Adjacent
		offsets.push_back(Vectori(i, 0, 0));
		offsets.push_back(Vectori(-i, 0, 0));
		offsets.push_back(Vectori(0, 0, i));
		offsets.push_back(Vectori(0, 0, -i));
		offsets.push_back(Vectori(0, i, 0));
		offsets.push_back(Vectori(0, -i, 0));
	
		//Lines
		for (int n = 1; n <= i; n++)
		{
			offsets.push_back(Vectori(i, n, 0));
			offsets.push_back(Vectori(i, -n, 0));
			offsets.push_back(Vectori(-i, n, 0));
			offsets.push_back(Vectori(-i, -n, 0));
			offsets.push_back(Vectori(i, 0, n));
			offsets.push_back(Vectori(i, 0, -n));
			offsets.push_back(Vectori(-i, 0, n));
			offsets.push_back(Vectori(-i, 0, -n));

			offsets.push_back(Vectori(n, i, 0));
			offsets.push_back(Vectori(-n, i, 0));
			offsets.push_back(Vectori(n, -i, 0));
			offsets.push_back(Vectori(-n, -i, 0));
			offsets.push_back(Vectori(0, i, n));
			offsets.push_back(Vectori(0, i, -n));
			offsets.push_back(Vectori(0, -i, n));
			offsets.push_back(Vectori(0, -i, -n));

			offsets.push_back(Vectori(n, 0, i));
			offsets.push_back(Vectori(-n, 0, i));
			offsets.push_back(Vectori(n, 0, -i));
			offsets.push_back(Vectori(-n, 0, -i));
			offsets.push_back(Vectori(0, n, i));
			offsets.push_back(Vectori(0, -n, i));
			offsets.push_back(Vectori(0, n, -i));
			offsets.push_back(Vectori(0, -n, -i));
		}
		
		//Corners
		for (int nx = 1; nx <= i; nx++)
			for (int ny = 1; ny <= i; ny++)
				for (int nz = 1; nz <= i; nz++)
			{
				offsets.push_back(Vectori(nx, nz, ny));
				offsets.push_back(Vectori(-nx, nz, ny));
				offsets.push_back(Vectori(nx, -nz, ny));
				offsets.push_back(Vectori(-nx, -nz, ny));
				offsets.push_back(Vectori(nx, nz, -ny));
				offsets.push_back(Vectori(-nx, nz, -ny));
				offsets.push_back(Vectori(nx, -nz, -ny));
				offsets.push_back(Vectori(-nx, -nz, -ny));
			}
	}

	return offsets;
}