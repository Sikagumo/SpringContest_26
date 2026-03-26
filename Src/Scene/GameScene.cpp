#include "GameScene.h"
#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Actor/ActorBase.h"
#include "../Object/Common/Transform.h"
#include "../Object/StageObj/StageObjBase.h"
#include "../Object/Stage/StageController.h"
#include "../Object/Stage/StageBase.h"
#include "../Object/Stage/StageMove.h"
#include "../Object/Stage/StageGravity.h"
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
	isExecuteSwaped_(false),
	SceneBase()
{
}

void GameScene::Init(void)
{
	// ステージ初期化
	stage_ = new StageController();
	stage_->Init();

	// ステージ状態登録
	Player::STAGE_TYPE pStageType = Player::STAGE_TYPE::MAX;
	if (stage_->GetStageType() == StageController::STAGE_TYPE::MOVE ||
		stage_->GetStageType() == StageController::STAGE_TYPE::MOVE3D)
	{
		pStageType = Player::STAGE_TYPE::MOVE;
	}
	else if (stage_->GetStageType() == StageController::STAGE_TYPE::GRAVITY ||
		stage_->GetStageType() == StageController::STAGE_TYPE::GRAVITY3D)
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


	// 各プレイヤーとゴールの XY 距離判定
	bool isP1Clear = AsoUtility::IsHitCircleXY(player1_->GetTransform().pos, 20.0f,
											   stage_->GetGoalPos(static_cast<int>(Player::PLAYER_NO::P1)),
											   GOAL_HIT_RANGE);
	bool isP2Clear = AsoUtility::IsHitCircleXY(player2_->GetTransform().pos, 20.0f,
											   stage_->GetGoalPos(static_cast<int>(Player::PLAYER_NO::P2)),
											   GOAL_HIT_RANGE);

	//二人が星に触れた状態になったらタイトルに
	if (stage_->GetIsTypeEquals(StageController::STAGE_TYPE::CLEAR))
	{
		sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	else if (isP1Clear && isP2Clear)
	{
		SetStageType();
		return;
	}
	
	// プレイヤー交代
	PlayerSwap();

	// カメラ更新
	Camera* camera = sceneMng_.GetCamera();
	camera->Update();
}

void GameScene::Draw(void)
{
	skyDome_->Draw();

	stage_->DrawPre();

	if (isExecuteSwaped_)
	{
		player1_->Draw();
	}
	else
	{
		player2_->Draw();
	}

	stage_->Draw();

	if (isExecuteSwaped_)
	{
		player2_->Draw();
	}
	else
	{
		player1_->Draw();
	}

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

void GameScene::PlayerSwap(void)
{
	if (!isSwapping_)
	{
		// 各プレイヤーとゴールの XY 距離判定
		bool isP1Clear = AsoUtility::IsHitCircleXY(player1_->GetTransform().pos, 20.0f,
											       stage_->GetGoalPos(static_cast<int>(Player::PLAYER_NO::P1)),
												   GOAL_HIT_RANGE);
		bool isP2Clear = AsoUtility::IsHitCircleXY(player2_->GetTransform().pos, 20.0f,
												   stage_->GetGoalPos(static_cast<int>(Player::PLAYER_NO::P2)),
												   GOAL_HIT_RANGE);

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

		//入れ替え入力の判定と実行
		bool executeSwap = false;

		// 1. 今どちらが権限を持っていて、かつ対応するボタンが押されたか
		if (currentSwapRight_ == SWAP_RIGHT::P1)
		{
			// P1が権限保持中：P1のチェンジボタンだけをチェック
			if (input_.IsTrgDown(InputManager::TYPE::PLAYER1_CHANGE, Input::JOYPAD_NO::PAD1))
			{
				executeSwap = true;
				isSwapping_ = true;
				swapTimer_ = 0;

				//移動開始時の両者の位置を保持する
				p1StartPos_ = player1_->GetTransform().pos;
				p2StartPos_ = player2_->GetTransform().pos;

				//目的地を保存する
				p1EndPos_ = p2StartPos_;
				p2EndPos_ = p1StartPos_;
			}
		}
		else if (currentSwapRight_ == SWAP_RIGHT::P2)
		{
			// P2が権限保持中：P2のチェンジボタンだけをチェック
			if (input_.IsTrgDown(InputManager::TYPE::PLAYER2_CHANGE, Input::JOYPAD_NO::PAD2))
			{
				executeSwap = true;
				isSwapping_ = true;
				swapTimer_ = 0;

				//移動開始時の両者の位置を保持する
				p1StartPos_ = player1_->GetTransform().pos;
				p2StartPos_ = player2_->GetTransform().pos;

				//目的地を保存する
				p1EndPos_ = p2StartPos_;
				p2EndPos_ = p1StartPos_;

			}
		}

	}
	else
	{
		// 入れ替え実行と権限の譲渡
		if (isSwapping_)
		{
			//	// Transformを参照で取得
			//	Transform& t1 = player1_->GetTransform();
			//	Transform& t2 = player2_->GetTransform();

			//	// 位置の入れ替え
			//	VECTOR temp = t1.pos;
			//	t1.pos = t2.pos;
			//	t2.pos = temp;

			//	// 物理挙動の安定化
			//	t1.prePos = t1.pos;
			//	t2.prePos = t2.pos;

			//	// 権限を交互に切り替える
			//	if (currentSwapRight_ == SWAP_RIGHT::P1)
			//	{
			//		currentSwapRight_ = SWAP_RIGHT::P2;
			//	}
			//	else
			//	{
			//		currentSwapRight_ = SWAP_RIGHT::P1;
			//	}
			//}

			// タイマー更新
			swapTimer_ += 1.0f;
			float t = swapTimer_ / SWAP_LIMIT_FRAME; // 0.0 ~ 1.0 に正規化

			if (t > 1.0f) t = 1.0f;

			// イージング関数の適用
			float easedT = 1.0f - powf(1.0f - t, 3.0f);

			// プレイヤー1の座標を更新
			Transform& t1 = player1_->GetTransform();
			t1.pos.x = p1StartPos_.x + (p1EndPos_.x - p1StartPos_.x) * easedT;
			t1.pos.y = p1StartPos_.y + (p1EndPos_.y - p1StartPos_.y) * easedT;
			t1.prePos = t1.pos;

			// プレイヤー2の座標を更新
			Transform& t2 = player2_->GetTransform();
			t2.pos.x = p2StartPos_.x + (p2EndPos_.x - p2StartPos_.x) * easedT;
			t2.pos.y = p2StartPos_.y + (p2EndPos_.y - p2StartPos_.y) * easedT;
			t2.prePos = t2.pos;

			// 移動完了判定
			if (t >= 1.0f)
			{
				// ここで権限を譲渡する
				isSwapping_ = false;
				currentSwapRight_ = (currentSwapRight_ == SWAP_RIGHT::P1)
							? SWAP_RIGHT::P2 : SWAP_RIGHT::P1;
			}
		}
	}
}

void GameScene::SetStageType(void)
{
	// ステージ当たり判定を削除
	player1_->RemoveHitCollider(ColliderBase::TAG::STAGE);
	player1_->RemoveHitCollider(ColliderBase::TAG::GOAL);
	player2_->RemoveHitCollider(ColliderBase::TAG::STAGE);
	player2_->RemoveHitCollider(ColliderBase::TAG::GOAL);

	// ステージ状態を進める
	stage_->ChangeStages();
	
	stage_->AddStageColliders(*player1_);
	stage_->AddStageColliders(*player2_);

	Player::STAGE_TYPE pStageType = Player::STAGE_TYPE::MAX;

	if (stage_->GetStageType() == StageController::STAGE_TYPE::MOVE ||
		stage_->GetStageType() == StageController::STAGE_TYPE::MOVE3D)
	{
		pStageType = Player::STAGE_TYPE::MOVE;
	}
	else if (stage_->GetStageType() == StageController::STAGE_TYPE::GRAVITY ||
			 stage_->GetStageType() == StageController::STAGE_TYPE::GRAVITY3D)
	{
		pStageType = Player::STAGE_TYPE::GRAVITY;
	}

	if (player1_ != nullptr)
	{
		VECTOR stagePos = stage_->GetPlayerPos(static_cast<int>(Player::PLAYER_NO::P1));
		player1_->Init(stagePos, pStageType);
	}
	if (player2_ != nullptr)
	{
		VECTOR stagePos = stage_->GetPlayerPos(static_cast<int>(Player::PLAYER_NO::P2));
		player2_->Init(stagePos, pStageType);
	}
}
