#include "StageBase.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"

StageBase::StageBase(void):
	sceneMng_(SceneManager::GetInstance()),
	resMng_(ResourceManager::GetInstance())
{
}
