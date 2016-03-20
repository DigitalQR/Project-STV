#include "Terrain.h"

Terrain::Terrain(unsigned int seed) : _SEED(seed)
{

}

Terrain::~Terrain() 
{
	delete MarchingCube::getMain();
}
