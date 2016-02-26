#include "RenderQueue.h"
#include <iostream>


RenderQueue::RenderQueue()
{
}


RenderQueue::~RenderQueue()
{
	cout << "\tDeleting static render queue." << endl;
	for (map<Model*, vector<Entity*>*>::iterator it = _static_queue.begin(); it != _static_queue.end(); it++)
	{
		delete _static_queue[it->first];
	}
}

void RenderQueue::addToStaticRenderQueue(Entity* e)
{
	if (_static_queue.find(e->model) != _static_queue.end()) 
	{
		//Found
		_static_queue[e->model]->push_back(e);
	}
	else 
	{
		//Not found
		vector<Entity*>* vec = new vector<Entity*>();
		vec->push_back(e);
		_static_queue[e->model] = vec;
	}

}
