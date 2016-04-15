#include "Terrain.h"
#include "Chunk.h"
#include "GameManager.h"
#include <set>

Terrain::Terrain(unsigned int seed) : _SEED(seed)
{
	_texture_atlas = GameManager::getMain()->texture_loader->LoadLinearPNG("Res/tileset.png");
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

void Terrain::PlaceResource(int x, int y, int z, resource_id resource, bool overwrite) 
{
	Vectori chunk_coords = Chunk::GetChunkCoordsOf(x, y, z);
	x -= chunk_coords.x * CHUNK_SIZE_X;
	y -= chunk_coords.y * CHUNK_SIZE_Y;
	z -= chunk_coords.z * CHUNK_SIZE_Z;

	Chunk* chunk = _chunk_loader->GetChunk(chunk_coords.x, chunk_coords.y, chunk_coords.z);
	
	if (!overwrite)
		if (IsSolid(chunk->GetResourceAt(x, y, z)))
			return;

	chunk->SetResourceAt(x, y, z, resource);


	set<Vectori> rebuild_chunks;
	rebuild_chunks.insert(Vectori(chunk_coords.x, chunk_coords.y, chunk_coords.z));
	Vectori extra_chunk(chunk_coords.x, chunk_coords.y, chunk_coords.z);

	if (x == CHUNK_SIZE_X - 1)
	{
		rebuild_chunks.insert(Vectori(chunk_coords.x + 1, chunk_coords.y, chunk_coords.z));
		extra_chunk.x++;
	}
	if (y == CHUNK_SIZE_Y - 1)
	{
		rebuild_chunks.insert(Vectori(chunk_coords.x, chunk_coords.y + 1, chunk_coords.z));
		extra_chunk.y++;
	}
	if (z == CHUNK_SIZE_Z - 1)
	{
		rebuild_chunks.insert(Vectori(chunk_coords.x, chunk_coords.y, chunk_coords.z + 1));
		extra_chunk.z++;
	}

	if (x == 0)
	{
		rebuild_chunks.insert(Vectori(chunk_coords.x - 1, chunk_coords.y, chunk_coords.z));
		extra_chunk.x--;
	}
	if (y == 0)
	{
		rebuild_chunks.insert(Vectori(chunk_coords.x, chunk_coords.y - 1, chunk_coords.z));
		extra_chunk.y--;
	}
	if (z == 0)
	{
		rebuild_chunks.insert(Vectori(chunk_coords.x, chunk_coords.y, chunk_coords.z - 1));
		extra_chunk.z--;
	}
	if (rebuild_chunks.find(extra_chunk) == rebuild_chunks.end())
		rebuild_chunks.insert(extra_chunk);
	

	for (Vectori chunk : rebuild_chunks)
	{
		_chunk_loader->GetChunk(chunk.x, chunk.y, chunk.z)->RebuildModel();
	}
}


void Terrain::PlaceResources(vector<Vectori>& coordinates, resource_id resource, bool overwrite) 
{
	set<Vectori> rebuild_chunks;

	for (Vectori coords: coordinates)
	{
		int x = coords.x;
		int y = coords.y;
		int z = coords.z;

		Vectori chunk_coords = Chunk::GetChunkCoordsOf(x, y, z);
		x -= chunk_coords.x * CHUNK_SIZE_X;
		y -= chunk_coords.y * CHUNK_SIZE_Y;
		z -= chunk_coords.z * CHUNK_SIZE_Z;

		Chunk* chunk = _chunk_loader->GetChunk(chunk_coords.x, chunk_coords.y, chunk_coords.z);

		if (!overwrite)
			if (IsSolid(chunk->GetResourceAt(x, y, z)))
				continue;

		chunk->SetResourceAt(x, y, z, resource);
		rebuild_chunks.insert(chunk_coords);

		Vectori extra_chunk(chunk_coords.x, chunk_coords.y, chunk_coords.z);

		if (x == CHUNK_SIZE_X - 1) 
		{
			rebuild_chunks.insert(Vectori(chunk_coords.x + 1, chunk_coords.y, chunk_coords.z));
			extra_chunk.x++;
		}
		if (y == CHUNK_SIZE_Y - 1)
		{
			rebuild_chunks.insert(Vectori(chunk_coords.x, chunk_coords.y + 1, chunk_coords.z));
			extra_chunk.y++;
		}
		if (z == CHUNK_SIZE_Z - 1)
		{
			rebuild_chunks.insert(Vectori(chunk_coords.x, chunk_coords.y, chunk_coords.z + 1));
			extra_chunk.z++;
		}

		if (x == 0)
		{
			rebuild_chunks.insert(Vectori(chunk_coords.x - 1, chunk_coords.y, chunk_coords.z));
			extra_chunk.x--;
		}
		if (y == 0)
		{
			rebuild_chunks.insert(Vectori(chunk_coords.x, chunk_coords.y - 1, chunk_coords.z));
			extra_chunk.y--;
		}
		if (z == 0)
		{
			rebuild_chunks.insert(Vectori(chunk_coords.x, chunk_coords.y, chunk_coords.z - 1));
			extra_chunk.z--;
		}

		if(rebuild_chunks.find(extra_chunk) == rebuild_chunks.end())
			rebuild_chunks.insert(extra_chunk);
	}
	
	for (Vectori chunk: rebuild_chunks) 
	{
		_chunk_loader->GetChunk(chunk.x, chunk.y, chunk.z)->RebuildModel();
	}
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

void Terrain::RemoveChunk(Chunk* chunk)
{
	_active_chunks.remove(chunk);
	chunk_flag++;
}

void Terrain::DeleteChunk(Chunk* chunk) 
{
	_chunks_to_delete.push_back(chunk);
	RemoveChunk(chunk);
	chunk_flag++;
}

void Terrain::VisualUpdate() 
{

	if (chunk_flag != render_flag)
	{
		render_flag = chunk_flag;

		list<Chunk*> delete_list = _chunks_to_delete;
		UpdateRenderedChunks(GetActiveChunks());

		//Delete chunks
		for (Chunk* chunk : delete_list)
		{
			_chunks_to_delete.remove(chunk);
			delete chunk;
		}
	}
}



void Terrain::UpdateRenderedChunks(list<Chunk*> active_chunks)
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

	_rendered_chunks.clear();
	copy(active_chunks.begin(), active_chunks.end(), back_inserter(_rendered_chunks));
}
