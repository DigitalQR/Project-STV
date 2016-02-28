#include "RenderQueue.h"
#include <iostream>


RenderQueue::~RenderQueue()
{
	cout << "\tDeleting render queue." << endl;
	for (map<const Model*, vector<Entity*>*>::iterator it = queue.begin(); it != queue.end(); it++)
	{
		delete queue[it->first];
	}
}

void RenderQueue::AddToRenderQueue(Entity* e)
{
	if (queue.find(e->model) != queue.end())
	{
		//Found
		queue[e->model]->push_back(e);
	}
	else
	{
		//Not found
		vector<Entity*>* vec = new vector<Entity*>();
		vec->push_back(e);
		queue[e->model] = vec;
	}

}

void RenderQueue::RemoveFromRenderQueue(Entity* e)
{
	vector<Entity*>* q = queue[e->model];

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
			queue.erase(e->model);
		}
		//Remove item in queue
		else
		{
			q->erase(q->begin() + index);
		}
	}
}
