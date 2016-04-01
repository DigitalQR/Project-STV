#pragma once
#include "Entity.h"
#include "OBJLoader.h"

class TestEntity : public Entity
{
public:
	TestEntity()
	{
	}

	vector<Model*> models;

	virtual void ReadyUp() 
	{
		map<string, Model*> models = OBJLoader::LoadOBJ("Res/Creatures/Player/T-Pose.obj");
		_texture = GameManager::getMain()->texture_loader->LoadPNG("Res/Creatures/Player/stv_charpal.png");

		for (map<string, Model*>::iterator it = models.begin(); it != models.end(); it++)
		{
			this->models.push_back(it->second);
			TexturedModel* tm = new TexturedModel(it->second, _texture);
			_textured_models.push_back(tm);
			this->AddElement(new Element3D(tm));
		}
	}


	~TestEntity()
	{
		for (TexturedModel* tm : _textured_models)
		{
			delete tm;
		}
		for (Model* model : models) 
		{
			delete model;
		}
	}

protected:
	list<TexturedModel*> _textured_models;
	GLuint _texture;
};
