#include "SceneBase.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"

SceneBase::SceneBase(void)
	: resMng_(ResourceManager::GetInstance())
	, sceneMng_(SceneManager::GetInstance())
	, input_(InputManager::GetInstance())
	, sound_(SoundManager::GetInstance())
	, uiText_{}
{
	uiText_.fill(-1);
	resMng_.LoadHandleIds(ResourceManager::SRC::IMGS_TEXT, uiText_.data());
}

SceneBase::~SceneBase(void)
{
}
