#include "RenderQueue.h"
#include <iostream>


RenderQueue::~RenderQueue()
{
	cout << "\tDeleting render queue." << endl;
	for (map<const TexturedModel*, vector<Entity*>*>::iterator it = queue.begin(); it != queue.end(); it++)
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
		vector<Entity*>* vec = new vector<Entity*>();
		vec->push_back(e);
		queue[e->textured_model] = vec;
	}

}

void RenderQueue::RemoveFromRenderQueue(Entity* e)
{
	vector<Entity*>* q = queue[e->textured_model];

	int index = -1;

	for (int i = 0; i < q->size(); i++) {
		if (q->at(i) == e)
		{
			index = i;
			break;
		}
	}

	//Has item been found in queue
	if (index != -1)
	{
		//Remove queue (Last item in queue)
		if (q->size() == 1)
		{
			queue.erase(e->textured_model);
		}
		//Remove item in queue
		else
		{
			q->erase(q->begin() + index);
		}
	}
}
