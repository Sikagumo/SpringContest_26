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
	:SceneBase(),
	isSelected_(false),
	state_(TITLE_STATE::SELECT_START)
{
}


void TitleScene::Init(void)
{

}

void TitleScene::Update(void)
{
	// シーン遷移
	if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void TitleScene::Draw(void)
{
 	DrawString(Application::SCREEN_HALF_X-100, Application::SCREEN_HALF_Y, 
			   "Spaceでゲームスタート", 0x0);
}

void TitleScene::Release(void)
{
}
