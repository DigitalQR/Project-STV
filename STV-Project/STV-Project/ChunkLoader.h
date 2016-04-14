#pragma once
#include "MemoryListener.h"
#include <thread>
#include <vector>
#include <array>
#include <list>
#include "TerrainTools.h"

using namespace std;


class Terrain;
class Chunk;


class ChunkLoader
{
public:
	const unsigned int view_distance = 4;
	const unsigned int unload_distance = 5;
	bool running = true;
	bool active = true;

	ChunkLoader(Terrain* terrain) : _terrain(terrain)
	{
	}

	~ChunkLoader();

	void Start()
	{
		thread load_thread(&ChunkLoader::loading, this);
		load_thread.detach();
	}

	Chunk* GetChunk(int x, int y, int z);


protected:
	void loading();

	void AddChunk(Chunk* chunk);
	void DeleteChunk(Chunk* chunk);

	bool LoadChunk(int x, int y, int z);


private:
	Terrain* _terrain;
	array<Chunk*, 64> _chunk_cache{ nullptr };
	list<Chunk*> _generated_list;
	list<Vectori> _active_chunk_coords;

	void UpdateCache(Chunk* new_front);

	vector<Vectori> GenerateOffsets(int view_distance);
};