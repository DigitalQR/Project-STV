#pragma once
#include "MemoryListener.h"
#include <thread>
#include <vector>
#include <array>

using namespace std;


class Terrain;
class Chunk;


class ChunkLoader
{
public:
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


private:
	Terrain* _terrain;
	array<Chunk*, 64> _chunk_cache{ nullptr };
	vector<Chunk*> _generated_list;

	void UpdateCache(Chunk* new_front);
};