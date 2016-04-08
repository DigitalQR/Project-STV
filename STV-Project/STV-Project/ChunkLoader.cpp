#include "ChunkLoader.h"
#include "Terrain.h"
#include "Chunk.h"


ChunkLoader::~ChunkLoader()
{
	for (Chunk* chunk : _generated_list)
		delete chunk;
}

void ChunkLoader::loading()
{
	AddChunk(GetChunk(0, 0, 0));
	AddChunk(GetChunk(0, 1, 0));
	AddChunk(GetChunk(0, 2, 0));
	AddChunk(GetChunk(0, 3, 0));
	AddChunk(GetChunk(0, 4, 0));


	for (int r = 1; r < 2; r++)
	{
		if (!running)
			break;

		for (int d = 0; d < r * 2; d++)
		{
			if (!running)
				break;

			for (int y = 0; y < 5; y++)
			{
				AddChunk(GetChunk(-r + d, y, r));
				AddChunk(GetChunk(r - d, y, -r));
				AddChunk(GetChunk(-r, y, -r + d));
				AddChunk(GetChunk(r, y, r - d));
			}
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