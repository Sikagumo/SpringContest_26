#include "GameScene.h"
#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Actor/ActorBase.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/StageMove.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Player/Player.h"
#include "../Object/Collider/ColliderBase.h"
#include "../Manager/Camera.h"


GameScene::GameScene(void):
	  skyDome_(nullptr),
	  stage_(nullptr),
	  SceneBase()
{
}

void GameScene::Init(void)
{
	stage_ = new StageMove();
	stage_->Init();

	skyDome_ = new SkyDome({});
	skyDome_->Init();

	Camera* camera = sceneMng_.GetCamera();
	camera->Init();
}

void GameScene::Update(void)
{

	// ƒV[ƒ“‘JˆÚ
#ifdef _DEBUG
	if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
#endif

	stage_->Update();

	skyDome_->Update();

	Camera* camera = sceneMng_.GetCamera();
	//camera->SetFollow(&player_->GetTransform());
	camera->Update();
}

void GameScene::Draw(void)
{
	skyDome_->Draw();

	stage_->Draw();

	stage_->DrawDebug();
}

void GameScene::Release(void)
{
	stage_->Release();
	delete stage_;

	skyDome_->Release();
	delete skyDome_;
}
