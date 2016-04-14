#include "Model.h"
#include "GameManager.h"

Model::~Model()
{
	GameManager::getMain()->model_loader->DeleteModelData(this);
}