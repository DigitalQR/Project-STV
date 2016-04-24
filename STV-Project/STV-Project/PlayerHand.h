#pragma once
#include "Component.h"
#include "BlockGlossary.h"
#include "ResourcePlacer.h"
#include "Terrain.h"
#include "GameManager.h"
#include "OBJLoader.h"


class PlayerHand : public Component 
{
private:

	TexturedModel* _hand_model;
	ResourcePlacer* _resource_placer;
public:
	static resource_id& GetCurrentItemID() 
	{
		static resource_id _current_item = RES_AIR;
		return _current_item;
	}


	void Start() 
	{
		_resource_placer = parent->GetComponent<ResourcePlacer>();
		_hand_model = new TexturedModel(OBJLoader::LoadOBJ("Res/PlayerHand.obj")["Hand"], Terrain::getTextureAtlas() );

		Element3D* element = new Element3D(_hand_model);
		element->Init(vec3(-0.45f, -0.35f, 0.5f), vec3(0, 0, 0), 1.0f);
		element->preferred_shader = GameManager::getMain()->master_renderer->hand_shader;

		parent->AddElement(element);
		element->setParent(nullptr);
	}

	~PlayerHand() 
	{
		delete _hand_model->model;
		delete _hand_model;
	}

	void LogicUpdate() {}

	void VisualUpdate() 
	{
		GetCurrentItemID() = _resource_placer->GetCurrentResource();
	}

};