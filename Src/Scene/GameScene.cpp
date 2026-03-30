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
	  skyDome_(nullptr)
	, stage_(nullptr)
	, player1_(PlayerParam()), player2_(PlayerParam())
	, SceneBase()
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
	player1_.player = new Player(Player::PLAYER_NO::P1, stagePos);
	player1_.player->Init(stagePos, pStageType);
	player1_.initialPos = stagePos;
	
	stagePos = stage_->GetPlayerPos(static_cast<int>(Player::PLAYER_NO::P2));
	player2_.player = new Player(Player::PLAYER_NO::P2, stagePos);
	player2_.player->Init(stagePos, pStageType);
	player2_.initialPos = stagePos;
	
	// ステージ当たり判定登録
	stage_->AddStageColliders(*player1_.player);
	stage_->AddStageColliders(*player2_.player);

	// スカイドーム
	skyDome_ = new SkyDome({});
	skyDome_->Init();
	
	// カメラ
	Camera* camera = sceneMng_.GetCamera();
	camera->Init();

	state_ = GAME_STATE::ACTIVE;
}

void GameScene::Update(void)
{
	// 一時停止状態の切替
	if (input_.IsTrgDown(InputManager::TYPE::PAUSE)
		&& state_ != GAME_STATE::CLEAR)
	{
		state_ = ((state_ == GAME_STATE::ACTIVE) ? GAME_STATE::PAUSE : GAME_STATE::ACTIVE);
	}
	
	// 一時停止時、以下の処理を終了
	if (state_ == GAME_STATE::PAUSE) { return; }

	// プレイヤー更新処理
	player1_.player->Update();
	player2_.player->Update();

	if (!isSwapping_ && !isRespawning_)
	{

		stage_->Update();

		skyDome_->Update();

		
		// 罠の衝突処理(罠に衝突時、以下の処理を終了)
		if (TrapProcess()) { return; };

		// ゴール処理(ゴール中は以下の処理を終了)
		if (GoalProcess()) { return; }

#ifdef _DEBUG
		VECTOR pos1 = player1_.player->GetTransform().pos;
		VECTOR pos2 = player2_.player->GetTransform().pos;

		if (AsoUtility::IsHitCircleXY(pos1, 40.0f, pos2, 40.0f))
		{

			printfDx("XY衝突中！\n");
		}
#endif

		//入れ替え入力の判定と実行
		bool executeSwap = false;

		//今どちらが権限を持っていて、かつ対応するボタンが押されたか
		if (currentSwapRight_ == SWAP_RIGHT::P1)
		{
			// P1が権限保持中：P1のチェンジボタンだけをチェック
			if (input_.IsTrgDown(InputManager::TYPE::PLAYER1_CHANGE, Input::JOYPAD_NO::PAD1))
			{
				executeSwap = true;
				isSwapping_ = true;
				swapTimer_ = 0;

				//移動開始時の両者の位置を保持する
				player1_.startPos = player1_.player->GetTransform().pos;
				player2_.startPos = player2_.player->GetTransform().pos;

				//目的地を保存する
				player1_.endPos = player2_.startPos;
				player2_.endPos = player1_.startPos;

				//交代中にゴールするのを阻止する
				return;
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
				player1_.startPos = player1_.player->GetTransform().pos;
				player2_.startPos = player2_.player->GetTransform().pos;

				//目的地を保存する
				player1_.endPos = player2_.startPos;
				player2_.endPos = player1_.startPos;

				//交代中にゴールするのを阻止する
				return;
			}
		}

	}
	else
	{
		// 入れ替え実行と権限の譲渡
		if (isSwapping_)
		{
			UpdateSwap();
		}

		if (isRespawning_)
		{
			UpdateRespawn();
		}
	}

	// カメラ更新
	Camera* camera = sceneMng_.GetCamera();
	camera->Update();
}

void GameScene::Draw(void)
{
	skyDome_->Draw();

	if (currentSwapRight_ == SWAP_RIGHT::P1)
	{
		player2_.player->Draw();
	}
	else
	{
		player1_.player->Draw();
	}

	stage_->Draw();

	if (currentSwapRight_ == SWAP_RIGHT::P1)
	{
		player1_.player->Draw();
	}
	else
	{
		player2_.player->Draw();
	}

	stage_->DrawDebug();
}

void GameScene::Release(void)
{
	stage_->Release();
	delete stage_;

	skyDome_->Release();
	delete skyDome_;

	player1_.player->Release();
	delete player1_.player;

	player2_.player->Release();
	delete player2_.player;
}

void GameScene::UpdateSwap(void)
{
	// タイマー更新
	swapTimer_ += 1.0f;
	float t = swapTimer_ / SWAP_LIMIT_FRAME; // 0.0 ～ 1.0 に正規化

	if (t > 1.0f) t = 1.0f;

	// イージング関数の適用
	float easedT = 1.0f - powf(1.0f - t, 3.0f);

	// プレイヤー1の座標を更新
	Transform& t1 = player1_.player->GetTransform();
	t1.pos.x = (player1_.startPos.x + (player1_.endPos.x - player1_.startPos.x) * easedT);
	t1.pos.y = (player1_.startPos.y + (player1_.endPos.y - player1_.startPos.y) * easedT);
	t1.pos.z = (player1_.startPos.z + (player1_.endPos.z - player1_.startPos.z) * easedT);
	t1.prePos = t1.pos;

	// プレイヤー2の座標を更新
	Transform& t2 = player2_.player->GetTransform();
	t2.pos.x = player2_.startPos.x + (player2_.endPos.x - player2_.startPos.x) * easedT;
	t2.pos.y = player2_.startPos.y + (player2_.endPos.y - player2_.startPos.y) * easedT;
	t2.pos.z = player2_.startPos.z + (player2_.endPos.z - player2_.startPos.z) * easedT;
	t2.prePos = t2.pos;

	// 移動完了判定
	if (t >= 1.0f)
	{
		isSwapping_ = false;

		//ここで権限を譲渡する
		currentSwapRight_ = (currentSwapRight_ == SWAP_RIGHT::P1)
			? SWAP_RIGHT::P2 : SWAP_RIGHT::P1;
	}
}

void GameScene::UpdateRespawn(void)
{
	//タイマー更新
	respawnTimer_ += 1.0f;
	float t = respawnTimer_ / RESPAWN_LIMIT_FRAME;

	if (t > 1.0f) t = 1.0f;

	//イージング関数の適用
	float easedT = 1.0f - powf(1.0f - t, 3.0f);

	//プレイヤー１の座標を補間
	Transform& t1 = player1_.player->GetTransform();
	t1.pos.x = player1_.deathPos.x + (player1_.initialPos.x - player1_.deathPos.x) * easedT;
	t1.pos.y = player1_.deathPos.y + (player1_.initialPos.y - player1_.deathPos.y) * easedT;
	t1.prePos = t1.pos;

	// プレイヤー2の座標を補間（死んだ場所 -> 初期位置）
	Transform& t2 = player2_.player->GetTransform();
	t2.pos.x = player2_.deathPos.x + (player2_.initialPos.x - player2_.deathPos.x) * easedT;
	t2.pos.y = player2_.deathPos.y + (player2_.initialPos.y - player2_.deathPos.y) * easedT;
	t2.prePos = t2.pos;

	// 完了判定
	if (t >= 1.0f)
	{
		isRespawning_ = false;
	}
}

bool GameScene::TrapProcess(void)
{
	/* トラップ判定 */
	bool ret = false;

	//親クラス StageBaseで定義されている GetTrapPos()を使用
	const auto& trapList = stage_->GetTrapPos();

	Player* targetPlayers[] = { player1_.player, player2_.player };

	for (Player* p : targetPlayers)
	{
		//プレイヤーの現在位置を取得
		VECTOR pPos = p->GetTransform().pos;

		//リストの中にあるトラップを一つずつ取り出して判定
		for (const VECTOR& tPos : trapList)
		{
			//判定：プレイヤー判定20.0f,トラップ半径25.0f
			if (AsoUtility::IsHitCircleXY(pPos, 20.0f, tPos, 60.0f))
			{
				isRespawning_ = true;
				respawnTimer_ = 0.0f;

				player1_.deathPos = player1_.player->GetTransform().pos;
				player2_.deathPos = player2_.player->GetTransform().pos;

				//他の処理を中断（ゴール判定など)
				return true;
			}
		}
	}
	return ret;
}

bool GameScene::GoalProcess(void)
{
	bool ret = false;

	// 判定の大きさ
	const float GOAL_HIT_RANGE = 80.0f;
	VECTOR goalPos = AsoUtility::VECTOR_ZERO;
	bool isP1Clear, isP2Clear;

	isP1Clear = isP2Clear = false;

	for (int i = 0; i < 2; i++)
	{
		// 各プレイヤーとゴールの XY 距離判定
		isP1Clear = AsoUtility::IsHitCircleXY(player1_.player->GetTransform().pos, 20.0f,
											  stage_->GetGoalPos(i), GOAL_HIT_RANGE);

		isP2Clear = AsoUtility::IsHitCircleXY(player2_.player->GetTransform().pos, 20.0f,
											  stage_->GetGoalPos((i + 1) % 2), GOAL_HIT_RANGE);

		// ゴール到達時、ループ終了
		if (isP1Clear && isP2Clear) { break; }
	}


	//二人が星に触れた状態になったらタイトルに
	if (isP1Clear && isP2Clear)
	{
		ret = true;

		if (stage_->GetIsTypeEquals(StageController::STAGE_TYPE::CLEAR))
		{
			sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
		else
		{
			SetStageType();
		}
	}

	return ret;
}

void GameScene::SetStageType(void)
{
	// ステージ当たり判定を削除
	player1_.player->RemoveHitCollider(ColliderBase::TAG::STAGE);
	player1_.player->RemoveHitCollider(ColliderBase::TAG::GOAL);
	player2_.player->RemoveHitCollider(ColliderBase::TAG::STAGE);
	player2_.player->RemoveHitCollider(ColliderBase::TAG::GOAL);

	// ステージ状態を進める
	stage_->ChangeStages();

	stage_->AddStageColliders(*player1_.player);
	stage_->AddStageColliders(*player2_.player);

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

	if (player1_.player != nullptr)
	{
		VECTOR stagePos = stage_->GetPlayerPos(static_cast<int>(Player::PLAYER_NO::P1));
		player1_.player->Init(stagePos, pStageType);
	}
	if (player2_.player != nullptr)
	{
		VECTOR stagePos = stage_->GetPlayerPos(static_cast<int>(Player::PLAYER_NO::P2));
		player2_.player->Init(stagePos, pStageType);
	}
}