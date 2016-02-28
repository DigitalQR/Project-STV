#pragma once
#include <map>
#include <vector>
#include <list>o
#include "Model.h"
#include "Entity.h"

using namespace std;

class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();
	void AddToRenderQueue(Entity* e);
	void RemoveFromRenderQueue(Entity* e);

private:
	map<const Model*, vector<Entity*>*> _queue;
};

