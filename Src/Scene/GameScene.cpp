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
//#include "../Object/Stage/StageGravity.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Player/Player.h"
#include "../Object/Collider/ColliderBase.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"

GameScene::GameScene(void):
	skyDome_(nullptr),
	stage_(nullptr),
	player1_(nullptr),
	player2_(nullptr),
	stageType_(STAGE_TYPE::MAX),
	SceneBase()
{
}

void GameScene::Init(void)
{
	// ステージ初期化
	SetStageType(STAGE_TYPE::MOVE);

	// ステージ状態登録
	Player::STAGE_TYPE pStageType = Player::STAGE_TYPE::MAX;
	if (stageType_ == STAGE_TYPE::MOVE)
	{
		pStageType = Player::STAGE_TYPE::MOVE;
	}
	else if (stageType_ == STAGE_TYPE::GRAVITY)
	{
		pStageType = Player::STAGE_TYPE::GRAVITY;
	}


	VECTOR stagePos = AsoUtility::VECTOR_ZERO;

	stagePos = stage_->GetPlayerPos(static_cast<int>(Player::PLAYER_NO::P1));
	player1_ = new Player(Player::PLAYER_NO::P1, stagePos);
	player1_->Init(stagePos, pStageType);
	
	stagePos = stage_->GetPlayerPos(static_cast<int>(Player::PLAYER_NO::P2));
	player2_ = new Player(Player::PLAYER_NO::P2, stagePos);
	player2_->Init(stagePos, pStageType);
	
	// ステージ当たり判定登録
	stage_->AddStageColliders(*player1_);
	stage_->AddStageColliders(*player2_);

	// スカイドーム
	skyDome_ = new SkyDome({});
	skyDome_->Init();
	
	// カメラ
	Camera* camera = sceneMng_.GetCamera();
	camera->Init();
}

void GameScene::Update(void)
{

	// シーン遷移
#ifdef _DEBUG
	if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
#endif

	stage_->Update();

	skyDome_->Update();

	player1_->Update();

	player2_->Update();


	// ゴール判定
	// 親クラス StageBase で定義されている GetGoalPos() を使用
	VECTOR goalPos = stage_->GetGoalPos();

	// 判定の大きさ
	float hitRange = 80.0f;

	// 各プレイヤーとゴールの XY 距離判定
	bool isP1Clear = AsoUtility::IsHitCircleXY(player1_->GetTransform().pos, 20.0f, goalPos, hitRange);
	bool isP2Clear = AsoUtility::IsHitCircleXY(player2_->GetTransform().pos, 20.0f, goalPos, hitRange);

	//二人が星に触れた状態になったらタイトルに
	if (isP1Clear && isP2Clear)
	{
		sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		return; 
	}

	VECTOR pos1 = player1_->GetTransform().pos;
	VECTOR pos2 = player2_->GetTransform().pos;

	if (AsoUtility::IsHitCircleXY(pos1, 40.0f, pos2, 40.0f))
	{
#ifdef _DEBUG
		printfDx("XY衝突中！\n");
#endif
	}

	if (input_.IsTrgDown(InputManager::TYPE::PLAYER1_CHANGE, Input::JOYPAD_NO::PAD1) ||
		input_.IsTrgDown(InputManager::TYPE::PLAYER2_CHANGE, Input::JOYPAD_NO::PAD2))
	{
		// Transformを参照で取得（←重要）
		Transform& t1 = player1_->GetTransform();
		Transform& t2 = player2_->GetTransform();

		// 位置を保存
		VECTOR temp = t1.pos;

		// 入れ替え
		t1.pos = t2.pos;
		t2.pos = temp;

		// 重力落下防止（これ入れないとワープ後に落ちる）
		t1.prePos = t1.pos;
		t2.prePos = t2.pos;
	}

	// カメラ更新
	Camera* camera = sceneMng_.GetCamera();
	//camera->SetFollow(&player_->GetTransform());
	camera->Update();

}

void GameScene::Draw(void)
{
	skyDome_->Draw();

	stage_->Draw();

	player1_->Draw();

	player2_->Draw();

	stage_->DrawDebug();
}

void GameScene::Release(void)
{
	stage_->Release();
	delete stage_;

	skyDome_->Release();
	delete skyDome_;

	player1_->Release();
	delete player1_;

	player2_->Release();
	delete player2_;
}

void GameScene::SetStageType(GameScene::STAGE_TYPE _type)
{
	// 同一時処理終了
	if (stageType_ == _type) { return; }

	Player::STAGE_TYPE pStageType = Player::STAGE_TYPE::MAX;
	stageType_ = _type;
	
	if (stage_ != nullptr)
	{
		stage_->Release();
	}

	// ステージ登録
	if (_type == STAGE_TYPE::MOVE)
	{
		stage_ = new StageMove();
		pStageType = Player::STAGE_TYPE::MOVE;
	}
	else if (_type == STAGE_TYPE::GRAVITY)
	{
		//stage_ = new StageGravity();	
		pStageType = Player::STAGE_TYPE::GRAVITY;
	}
	if (player1_ != nullptr) { player1_->SetGameStageType(pStageType); }
	if (player2_ != nullptr) { player2_->SetGameStageType(pStageType); }

	stage_->Init();
}
