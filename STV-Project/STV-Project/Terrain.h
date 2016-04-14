#pragma once
#include "MemoryListener.h"
#include "ChunkLoader.h"
#include "Entity.h"
#include "Shader.h"
#include <list>

#include "Dependencies\glm\common.hpp"
#include "Dependencies\glew\glew.h"


using namespace glm;


class Chunk;


class Terrain
{
public:
	Terrain(unsigned int seed);
	~Terrain();

	unsigned int GetSeed() { return _SEED; }
	
	void VisualUpdate();
	list<Chunk*> GetActiveChunks() { return _active_chunks; }

	void AddChunk(Chunk* chunk);
	void RemoveChunk(Chunk* chunk);
	void DeleteChunk(Chunk* chunk);

	void PlaceResource(int x, int y, int z, resource_id resource, bool overwrite);
	void PlaceResources(vector<Vectori>& coordinates, resource_id resource, bool overwrite);

	void StartChunkLoading();
	void StopChunkLoading();

	ChunkLoader* GetChunkLoader() {
		return _chunk_loader;
	}

protected:
	unsigned int chunk_flag = 0;
	unsigned int render_flag = 0;

	void UpdateRenderedChunks(list<Chunk*> active_chunks);

private:
	Shader* _terrain_shader;
	GLuint _texture_atlas;
	ChunkLoader* _chunk_loader = new ChunkLoader(this);

	const unsigned int _SEED;
	list<Chunk*> _active_chunks;
	list<Chunk*> _rendered_chunks;
	list<Chunk*> _chunks_to_delete;
};
