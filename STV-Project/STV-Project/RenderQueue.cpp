#include "RenderQueue.h"
#include <iostream>


RenderQueue::~RenderQueue()
{
	cout << "\tDeleting render queue." << endl;
	for (map<const TexturedModel*, list<Entity*>*>::iterator it = queue.begin(); it != queue.end(); it++)
	{
		delete queue[it->first];
	}
}

void RenderQueue::AddToRenderQueue(Entity* e)
{
	if (queue.find(e->textured_model) != queue.end())
	{
		//Found
		queue[e->textured_model]->push_back(e);
	}
	else
	{
		//Not found
		list<Entity*>* vec = new list<Entity*>();
		vec->push_back(e);
		queue[e->textured_model] = vec;
	}

}

void RenderQueue::RemoveFromRenderQueue(Entity* e)
{
	list<Entity*>* q = queue[e->textured_model];

	bool contains = (find(q->begin(), q->end(), e)) != q->end();

	if (contains)
	{
		//Remove queue (Last item in queue)
		if (q->size() == 1)
		{
			queue.erase(e->textured_model);
		}
		//Remove item in queue
		else
		{
			q->remove(e);
		}
	}
}
