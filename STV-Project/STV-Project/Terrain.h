#pragma once
#include "MemoryListener.h"
#include "BlockGlossary.h"


using namespace std;


class Terrain
{
public:
	Terrain(unsigned int seed);
	~Terrain();

	unsigned int GetSeed() { return _SEED; }


private:
	const unsigned int _SEED;


};