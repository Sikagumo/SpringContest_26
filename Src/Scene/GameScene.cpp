#include "GameScene.h"
#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Actor/ActorBase.h"
#include "../Object/Stage/Stage.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Player/Player.h"
#include "../Object/Collider/ColliderBase.h"
#include "../Manager/Camera.h"


GameScene::GameScene(void):
	  skyDome_(nullptr),
	  SceneBase()
{
}

void GameScene::Init(void)
{		
	//skyDome_ = new SkyDome(player_->GetTransform());
	//skyDome_->Init();

	//Camera* camera = sceneMng_.GetCamera();
	//camera->SetFollow(&player_->GetTransform());
	//camera->Init();
	//camera->AddHitCollider(stageCollider);
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


	//skyDome_->Update();

	//Camera* camera = sceneMng_.GetCamera();
	//camera->SetFollow(&player_->GetTransform());
	//camera->Update();
}

void GameScene::Draw(void)
{
	//skyDome_->Draw();
}

void GameScene::Release(void)
{
	//skyDome_->Release();
	//delete skyDome_;
}
