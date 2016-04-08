#include "Terrain.h"
#include "Chunk.h"
#include "GameManager.h"

Terrain::Terrain(unsigned int seed) : _SEED(seed)
{
	_texture_atlas = GameManager::getMain()->texture_loader->LoadPNG("Res/World/grass_tile.png");
	_terrain_shader = GameManager::getMain()->master_renderer->terrain_shader;
}

Terrain::~Terrain()
{
	cout << "Deleting terrain..";
	_chunk_loader->running = false;
	//Wait for chunk loader to finish
	while (true)
	{
		if (!_chunk_loader->active)
		{
			break;
		}
		else 
		{
			cout << "";
		}
	}
	
	_active_chunks.clear();
	_rendered_chunks.clear();
	delete _chunk_loader;
	cout << " done." << endl;
}

void Terrain::StartChunkLoading()
{
	_chunk_loader->Start();
}

void Terrain::StopChunkLoading()
{
	_chunk_loader->running = false;
}

void Terrain::AddChunk(Chunk* chunk)
{
	_active_chunks.push_back(chunk);
	chunk_flag++;
}

void Terrain::VisualUpdate() 
{
	if (chunk_flag != render_flag)
	{
		render_flag = chunk_flag;
		UpdateRenderedChunks(GetActiveChunks());
	}
}

void Terrain::UpdateRenderedChunks(vector<Chunk*> active_chunks) 
{
	//Remove
	for (Chunk* current_chunk : _rendered_chunks)
	{
		if (current_chunk->GetEmptyModelFlag())
			continue;

		bool in = false;
		for (Chunk* active_chunk : active_chunks)
		{
			if (current_chunk == active_chunk)
			{
				in = true;
				break;
			}
		}

		//Remove from render queue
		if (!in)
		{
			_terrain_shader->RemoveFromRender(current_chunk->GetElement());
		}
	}

	//Add
	for (Chunk* current_chunk : active_chunks)
	{
		if (current_chunk->GetEmptyModelFlag())
			continue;

		bool in = false;
		for (Chunk* rendered_chunk : _rendered_chunks)
		{
			if (current_chunk == rendered_chunk)
			{
				in = true;
				break;
			}
		}

		if (!in) 
		{
			current_chunk->BuildModel();
			Element3D* element = current_chunk->GetElement();
			element->textured_model->texture = _texture_atlas;
			_terrain_shader->AddForRender(element);
		}

	}

	_rendered_chunks = active_chunks;
}
