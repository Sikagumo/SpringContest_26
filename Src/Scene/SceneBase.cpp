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
{
	for (int& ui : uiText_) { ui = -1; }

 	resMng_.LoadHandleIds(ResourceManager::SRC::IMGS_TEXT, uiText_);
}

SceneBase::~SceneBase(void)
{
}

void SceneBase::Init(void)
{
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}
