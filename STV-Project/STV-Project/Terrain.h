#pragma once
#include "MemoryListener.h"
#include "Chunk.h"
#include "Entity.h"
#include "Shader.h"

#include "Dependencies\glm\common.hpp"
#include "Dependencies\glew\glew.h"
#include <thread>


using namespace glm;

class Chunk;
class Terrain;


class ChunkLoader
{
public:
	bool running = true;
	bool active = true;

	ChunkLoader(Terrain* terrain) : _terrain(terrain)
	{
	}

	void Start()
	{
		thread load_thread(&ChunkLoader::loading, this);
		load_thread.detach();
	}

protected:
	void loading();

	void AddChunk(Chunk* chunk);


private:
	Terrain* _terrain;
};


class Terrain
{
public:
	Terrain(unsigned int seed);
	~Terrain();

	unsigned int GetSeed() { return _SEED; }
	
	void VisualUpdate();
	vector<Chunk*> GetActiveChunks() { return _active_chunks; }

	void AddChunk(Chunk* chunk);

	void StartChunkLoading();
	void StopChunkLoading();

protected:
	unsigned int chunk_flag = 0;
	unsigned int render_flag = 0;

	void UpdateRenderedChunks(vector<Chunk*> active_chunks);

private:
	Shader* _terrain_shader;
	GLuint _texture_atlas;
	ChunkLoader* _chunk_loader = new ChunkLoader(this);

	const unsigned int _SEED;
	vector<Chunk*> _active_chunks;
	vector<Chunk*> _rendered_chunks;	
};
