#pragma once
#include <map>
#include <vector>
#include "Model.h"
#include "Entity.h"

using namespace std;

class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();
	void addToStaticRenderQueue(Entity* e);

private:
	map<Model*, vector<Entity*>*> _static_queue;
};

