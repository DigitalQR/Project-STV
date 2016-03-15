#pragma once
#include "Entity.h"
#include "OBJLoader.h"

class TestEntity : public Entity
{
public:
	TestEntity()
	{
	}

	virtual void ReadyUp() 
	{
		//map<string, Model*> models = OBJLoader::LoadOBJ("Res/Creatures/Player/T-Pose.obj");
		//_texture = GameManager::getMain()->texture_loader->LoadPNG("Res/Creatures/Player/stv_charpal.png");

		//for (map<string, Model*>::iterator it = models.begin(); it != models.end(); it++)
		//{
		//	TexturedModel* tm = new TexturedModel(it->second, _texture);
		//	_textured_models.push_back(tm);
		//	this->AddElement(new Element3D(tm));
		//}
	}


	~TestEntity()
	{
		for (TexturedModel* tm : _textured_models)
		{
			//delete tm->model;
			delete tm;
		}
	}

protected:
	list<TexturedModel*> _textured_models;
	GLuint _texture;
};

#include "Chunk.h"

class TestEntity2 : public TestEntity 
{

public:
	~TestEntity2() 
	{
		for (Chunk* chunk: _chunks)
			delete chunk;
	}

	void ReadyUp() override
	{
		Terrain terrain(1297);

		_texture = GameManager::getMain()->texture_loader->LoadPNG("Res/World/grass_tile.png");

		for (int y = -1; y < 2; y++)
			for (int x = -1; x < 2; x++)
			{
				Chunk* chunk = new Chunk(&terrain, x, y);
				chunk->BuildTerrain();
				_chunks.push_back(chunk);

				TexturedModel* tm = new TexturedModel(chunk->GetTerrainModel(), _texture);
				this->AddElement(new Element3D(tm));
				_textured_models.push_back(tm);
			}
	}

private:
	vector<Chunk*> _chunks;

};