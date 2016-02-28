#include "RenderQueue.h"
#include <iostream>


RenderQueue::RenderQueue()
{
}


RenderQueue::~RenderQueue()
{
	cout << "\tDeleting render queue." << endl;
	for (map<const Model*, vector<Entity*>*>::iterator it = _queue.begin(); it != _queue.end(); it++)
	{
		delete _queue[it->first];
	}
}

void RenderQueue::AddToRenderQueue(Entity* e)
{
	if (_queue.find(e->model) != _queue.end())
	{
		//Found
		_queue[e->model]->push_back(e);
	}
	else
	{
		//Not found
		vector<Entity*>* vec = new vector<Entity*>();
		vec->push_back(e);
		_queue[e->model] = vec;
	}

}

void RenderQueue::RemoveFromRenderQueue(Entity* e)
{
	vector<Entity*>* queue = _queue[e->model];

	int index = -1;

	for (int i = 0; i < queue->size(); i++) {
		if (queue->at(i) == e)
		{
			index = i;
			break;
		}
	}

	//Has item been found in queue
	if (index != -1)
	{
		//Remove queue (Last item in queue)
		if (queue->size() == 1)
		{
			_queue.erase(e->model);
		}
		//Remove item in queue
		else
		{
			queue->erase(queue->begin() + index);
		}
	}
}
