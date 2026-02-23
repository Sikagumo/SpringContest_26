#include "GameScene.h"
#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Actor/ActorBase.h"
#include "../Object/Common/Transform.h"
#include "../Object/StageObj/StageObjBase.h"
#include "../Object/Stage/StageBase.h"
#include "../Object/Stage/StageMove.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Player/Player.h"
#include "../Object/Collider/ColliderBase.h"
#include "../Manager/Camera.h"


GameScene::GameScene(void) :
	skyDome_(nullptr),
	stage_(nullptr),
	  SceneBase()
{
	for (int i = 0; i < 2; i++)
	{
		temp_[i] = nullptr;
	}
}

void GameScene::Init(void)
{
	stage_ = new StageMove();
	stage_->Init();
	

	skyDome_ = new SkyDome({});
	skyDome_->Init();

	for (int i = 0; i < 2; i++)
	{
		temp_[i] = new Player();
		temp_[i]->Init(stage_->GetPlayerPos(i));

		// “–‚½‚è”»’è“o˜^
		for (auto& stageObjList : stage_->GetStageObjects())
		{
			for (auto& stageObj : stageObjList)
			{
				//temp_[i]->AddHitCollider(stageObj->GetOwnCollider());
			}
		}
	}

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

	//stage_->Update();

	skyDome_->Update();

	for (int i = 0; i < 2; i++)
	{
		temp_[i]->Update();
	}
	Camera* camera = sceneMng_.GetCamera();
	//camera->SetFollow(&player_->GetTransform());
	camera->Update();
}

void GameScene::Draw(void)
{
	skyDome_->Draw();

	stage_->Draw();

	stage_->DrawDebug();

	for (int i = 0; i < 2; i++)
	{
		temp_[i]->Draw();
	}
}

void GameScene::Release(void)
{
	stage_->Release();
	delete stage_;

	skyDome_->Release();
	delete skyDome_;

	for (int i = 0; i < 2; i++)
	{
		temp_[i]->Release();
		delete temp_[i];
	}
}
