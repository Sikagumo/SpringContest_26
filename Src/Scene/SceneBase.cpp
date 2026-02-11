#include "SceneBase.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"

SceneBase::SceneBase(void) 
	: 
	resMng_(ResourceManager::GetInstance()),
	sceneMng_(SceneManager::GetInstance()),
	input_(InputManager::GetInstance())
{
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
