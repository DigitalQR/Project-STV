#include "RenderQueue.h"
#include <iostream>


RenderQueue::~RenderQueue()
{
	cout << "\tDeleting render queue." << endl;
	for (map<const TexturedModel*, list<Element3D*>*>::iterator it = queue.begin(); it != queue.end(); it++)
	{
		delete it->second;
	}
	cout << "\tDone." << endl;
}

void RenderQueue::Clear() 
{
	for (map<const TexturedModel*, list<Element3D*>*>::iterator it = queue.begin(); it != queue.end(); it++)
	{
		delete it->second;
	}
	queue.clear();
}

void RenderQueue::AddToRenderQueue(Element3D* e)
{
	if (queue.find(e->textured_model) != queue.end())
	{
		//Found
		queue[e->textured_model]->push_back(e);
	}
	else
	{
		//Not found
		list<Element3D*>* vec = new list<Element3D*>();
		vec->push_back(e);
		queue[e->textured_model] = vec;
	}

}

void RenderQueue::RemoveFromRenderQueue(Element3D* e)
{
	list<Element3D*>* q = queue[e->textured_model];

	bool contains = (find(q->begin(), q->end(), e)) != q->end();

	if (contains)
	{
		//Remove queue (Last item in queue)
		if (q->size() == 1)
		{
			queue.erase(e->textured_model);
			//delete q;
		}
		//Remove item in queue
		else
		{
			q->remove(e);
		}
	}
}
