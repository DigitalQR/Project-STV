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


class Terrain
{
public:
	Terrain(unsigned int seed);
	~Terrain();

	unsigned int GetSeed() { return _SEED; }
	
	void VisualUpdate();
	vector<Chunk*> GetActiveChunks() { return _active_chunks; }

protected:
	unsigned int chunk_flag = 0;
	unsigned int render_flag = 0;

	void UpdateRenderedChunks(vector<Chunk*> active_chunks);

	void AddChunk(Chunk* chunk);

private:
	Shader* _terrain_shader;
	Entity* _follow;
	GLuint _texture_atlas;

	const unsigned int _SEED;
	vector<Chunk*> _active_chunks;
	vector<Chunk*> _rendered_chunks;	
};