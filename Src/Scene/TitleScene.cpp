#include "TitleScene.h"
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/SkyDome/SkyDome.h"


TitleScene::TitleScene(void)
	:SceneBase()
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{

}

void TitleScene::Update(void)
{
	// ÉVÅ[ÉìëJà⁄
	if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void TitleScene::Draw(void)
{

}

void TitleScene::Release(void)
{
}
