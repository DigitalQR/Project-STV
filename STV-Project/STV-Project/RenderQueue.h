#pragma once
#include "MemoryListener.h"
#include "Element3D.h"
#include <map>
#include <list>

using namespace std;

class RenderQueue
{
public:
	~RenderQueue();
	void AddToRenderQueue(Element3D* e);
	void RemoveFromRenderQueue(Element3D* e);
	map<const TexturedModel*, list<Element3D*>*> queue;

};