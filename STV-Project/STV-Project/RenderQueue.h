#pragma once
#include "MemoryListener.h"
#include <map>
#include "Entity.h"


class RenderQueue
{
public:
	~RenderQueue();
	void AddToRenderQueue(Entity* e);
	void RemoveFromRenderQueue(Entity* e);
	map<const TexturedModel*, vector<Entity*>*> queue;

};