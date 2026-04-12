#include "GameScene.h"
#include <DxLib.h>
#include <cmath>
#include <functional>
#include <array>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"
#include "../Object/Actor/ActorBase.h"
#include "../Object/Common/Transform.h"
#include "../Object/StageObj/StageObjBase.h"
#include "../Object/StageObj/StageObjTrap.h"
#include "../Object/Stage/StageController.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Player/Player.h"
#include "../Object/Collider/ColliderBase.h"
#include "../Common/Perform.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"

GameScene::GameScene(void)
	: SceneBase()
	, state_(GAME_STATE::NONE)
	, skyDome_(nullptr)
	, stage_(nullptr)
	, player1_(PlayerParam()), player2_(PlayerParam())
	, gameTimer_(GAME_TIME), performTime_(0.0f), isPerform_(false)
	, isGameTimeActive_(false), currentSwapRight_(SWAP_RIGHT::P1)
	, curInfoNum_(-1), preStageType_(-1)
{
	for (int& time : timeText_) { time = -1; }

	resMng_.LoadHandleIds(ResourceManager::SRC::IMGS_TEXT_TIME, timeText_);
	
	for (int& image : infoImages_)
	{
		image = -1;
	}
	resMng_.LoadHandleIds(ResourceManager::SRC::IMGS_INFO, infoImages_);
}

void GameScene::Init(void)
{
	currentSwapRight_ = SWAP_RIGHT::P1;

	// ステージ初期化
	stage_ = new StageController(sceneMng_.GetIsStageMove());
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
	player1_.player->SetAuthority(true);
	
	stagePos = stage_->GetPlayerPos(static_cast<int>(Player::PLAYER_NO::P2));
	player2_.player = new Player(Player::PLAYER_NO::P2, stagePos);
	player2_.player->Init(stagePos, pStageType);
	player2_.player->SetAuthority(false);
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

	isGameTimeActive_ = false;
	gameTimer_ = GAME_TIME;

	isPerform_ = false;

	ChangeState(GAME_STATE::INFO);
}

void GameScene::Update(void)
{
	// 状態別更新処理
	updateGameStateProc_();

	// 一時停止時、以下の処理を終了
	if (state_ == GAME_STATE::PAUSE
		|| state_ == GAME_STATE::ACTIVE && performTime_ >= 0.0f
		|| state_ == GAME_STATE::GOAL
		|| state_ == GAME_STATE::INFO
		|| state_ == GAME_STATE::INFO3D)
	{ return; }


	if (stage_->GetIsStageClear() || gameTimer_ <= 0.0f) { return; }

	// 時間減少
	if (isGameTimeActive_)
	{
		gameTimer_ -= sceneMng_.GetDeltaTime();

		if (gameTimer_ < 0.0f) { isGameTimeActive_ = false; }
	}


	// プレイヤー更新処理
	if (!player1_.player->GetIsGoal()) { player1_.player->Update(); }
	if (!player2_.player->GetIsGoal()) { player2_.player->Update(); }

	if (!isSwapping_ && !isRespawning_)
	{
		stage_->Update();

		skyDome_->Update();

		
		// 罠の衝突処理(罠に衝突時、以下の処理を終了)
		if (TrapProcess()) { return; };

		// ゴール処理(ゴール中は以下の処理を終了)
		if (GoalProcess()) { return; }

		//入れ替え入力の判定と実行
		bool executeSwap = false;

		//今どちらが権限を持っていて、かつ対応するボタンが押されたか
		if (input_.IsTrgDown(InputManager::TYPE::PLAYER1_CHANGE, Input::JOYPAD_NO::PAD1)
			&& currentSwapRight_ == SWAP_RIGHT::P1
			|| input_.IsTrgDown(InputManager::TYPE::PLAYER2_CHANGE, Input::JOYPAD_NO::PAD2)
			&& currentSwapRight_ == SWAP_RIGHT::P2)
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

			player1_.player->SetIsGoal(false);
			player2_.player->SetIsGoal(false);

			sound_.Play(static_cast<int>(ResourceManager::SRC::SE_CHANGE), false, true);

			//交代中にゴールするのを阻止する
			return;
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

	stage_->DrawPre();

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

	DrawTimer();

	DrawInfo();

	if (state_ == GAME_STATE::PAUSE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x0, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 1.0, 0.0f,
					  uiText_[static_cast<int>(UI_TEXT::PAUSE_MAIN)], true);

		DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 250, 1.0, 0.0f
					  , uiText_[static_cast<int>(UI_TEXT::PAUSE_SUB)], true);

		DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 350, 1.0, 0.0
					  , uiText_[static_cast<int>(UI_TEXT::DECISION_SUB)], true);
	}

	if (state_ == GAME_STATE::GAME_CLEAR
		|| state_ == GAME_STATE::GAME_OVER)
	{
		unsigned int color = 0;
		int uiType = -1;

		if (state_ == GAME_STATE::GAME_CLEAR)
		{
			color = 0xffff00;
			uiType = static_cast<int>(UI_TEXT::GAME_CLEAR);
		}
		else
		{
			color = 0x0;
			uiType = static_cast<int>(UI_TEXT::GAME_OVER);
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-150);
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, color, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		int y = Application::SCREEN_HALF_Y - 100;
		DrawRotaGraph(Application::SCREEN_HALF_X, y, 1.0, 0.0
					  , uiText_[uiType], true);

		if (performTime_ <= (TIME_GAME_END / 3))
		{
			y += 250;
			DrawRotaGraph(Application::SCREEN_HALF_X, y, 1.0, 0.0
				, uiText_[static_cast<int>(UI_TEXT::DECISION_SUB)], true);
		}
		if (performTime_ <= (TIME_GAME_END / 5))
		{
			y += 125;
			DrawRotaGraph(Application::SCREEN_HALF_X, y, 1.0, 0.0
				, uiText_[static_cast<int>(UI_TEXT::PAUSE_SUB)], true);
		}
	}
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
	/* 交代処理 */
	// タイマー更新
	swapTimer_ += 1.0f;

	// イージング関数の適用
	float easingNum = UtilityCommon::EasingLine(swapTimer_, SWAP_LIMIT_FRAME, 3.0f);

	// プレイヤー1の座標を更新
	Transform& t1 = player1_.player->GetTransform();
	t1.pos.x = (player1_.startPos.x + (player1_.endPos.x - player1_.startPos.x) * easingNum);
	t1.pos.y = (player1_.startPos.y + (player1_.endPos.y - player1_.startPos.y) * easingNum);
	t1.pos.z = (player1_.startPos.z + (player1_.endPos.z - player1_.startPos.z) * easingNum);
	t1.prePos = t1.pos;

	// プレイヤー2の座標を更新
	Transform& t2 = player2_.player->GetTransform();
	t2.pos.x = player2_.startPos.x + (player2_.endPos.x - player2_.startPos.x) * easingNum;
	t2.pos.y = player2_.startPos.y + (player2_.endPos.y - player2_.startPos.y) * easingNum;
	t2.pos.z = player2_.startPos.z + (player2_.endPos.z - player2_.startPos.z) * easingNum;
	t2.prePos = t2.pos;

	// 移動完了判定
	if (swapTimer_ >= SWAP_LIMIT_FRAME)
	{
		isSwapping_ = false;

		//ここで権限を譲渡する
		currentSwapRight_ = (currentSwapRight_ == SWAP_RIGHT::P1)
							  ? SWAP_RIGHT::P2 : SWAP_RIGHT::P1;

		player1_.player->SetAuthority(currentSwapRight_ == SWAP_RIGHT::P1);
		player2_.player->SetAuthority(currentSwapRight_ == SWAP_RIGHT::P2);
	}
}

void GameScene::UpdateRespawn(void)
{
	/* リスタート処理 */
	//タイマー更新
	respawnTimer_ += 1.0f;

	//イージング関数の適用
	float easingNum = UtilityCommon::EasingLine(respawnTimer_, RESPAWN_LIMIT_FRAME, 3.0f);

	//プレイヤー１の座標を補間
	Transform& t1 = player1_.player->GetTransform();
	t1.pos.x = player1_.deathPos.x + (player1_.initialPos.x - player1_.deathPos.x) * easingNum;
	t1.pos.y = player1_.deathPos.y + (player1_.initialPos.y - player1_.deathPos.y) * easingNum;
	t1.pos.z = player1_.deathPos.z + (player1_.initialPos.z - player1_.deathPos.z) * easingNum;
	t1.prePos = t1.pos;

	// プレイヤー2の座標を補間（死んだ場所 -> 初期位置）
	Transform& t2 = player2_.player->GetTransform();
	t2.pos.x = player2_.deathPos.x + (player2_.initialPos.x - player2_.deathPos.x) * easingNum;
	t2.pos.y = player2_.deathPos.y + (player2_.initialPos.y - player2_.deathPos.y) * easingNum;
	t2.pos.z = player2_.deathPos.z + (player2_.initialPos.z - player2_.deathPos.z) * easingNum;
	t2.prePos = t2.pos;

	// 完了判定
	if (respawnTimer_ >= RESPAWN_LIMIT_FRAME)
	{
		isRespawning_ = false;
		respawnTimer_ = 0.0f;

		// プレイヤーの見た目をもとに戻す
		player1_.player->SetIsChangeModel(false);
		player2_.player->SetIsChangeModel(false);

		currentSwapRight_ = SWAP_RIGHT::P1;
		player1_.player->SetAuthority(true);
		player2_.player->SetAuthority(false);
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
		for (auto& tPos : trapList)
		{
			if (AsoUtility::IsHitSpheres(pPos, Player::COL_CAPSULE_RADIUS,
										 tPos, StageObjTrap::COLLISION_RADIUS))
			{
				if(fabsf(pPos.z - tPos.z) < 40.0f)
				isRespawning_ = true;
				respawnTimer_ = 0.0f;

				// プレイヤーの見た目を変更
				player1_.player->SetIsChangeModel(true);
				player2_.player->SetIsChangeModel(true);

				player1_.player->SetIsGoal(false);
				player2_.player->SetIsGoal(false);

				player1_.deathPos = player1_.player->GetTransform().pos;
				player2_.deathPos = player2_.player->GetTransform().pos;

				for (int i = 0; i < GetJoypadNum(); i++)
				{
					unsigned int padNum = ((i == 0) ? DX_INPUT_PAD1 : 0);
					padNum = ((i == 1) ? DX_INPUT_PAD2 : padNum);
					padNum = ((i == 2) ? DX_INPUT_PAD3 : padNum);
					padNum = ((i == 3) ? DX_INPUT_PAD4 : padNum);

					const float VIBRATION_TIME = 0.2f;
					StartJoypadVibration(padNum, 1000, static_cast<int>(VIBRATION_TIME * 1000.0f));
				}

				
				sceneMng_.GetPerform().SetHitStop(HIT_STOP_TRAP);

				// ダメージSE再生
				sound_.Play(static_cast<int>(ResourceManager::SRC::SE_DAMAGE), false, true);

				//他の処理を中断（ゴール判定など)
				return true;
			}
		}
	}
	return ret;
}

bool GameScene::GoalProcess(void)
{
	// 更新処理の処理をスキップするか否か
	bool isProcessStop = false;
	
	for (int i = 0; i < static_cast<int>(Player::PLAYER_NO::MAX); i++)
	{
		// 各プレイヤーとゴールの XY 距離判定
		if (!player1_.player->GetIsGoal()
			&& AsoUtility::IsHitSpheres(player1_.player->GetTransform().pos, 0.0f
										, stage_->GetGoalPos(i), GOAL_HIT_RANGE))
		{
			player1_.player->SetIsGoal(true);
		}

		if (!player2_.player->GetIsGoal()
			&& AsoUtility::IsHitSpheres(player2_.player->GetTransform().pos, 0.0f
										, stage_->GetGoalPos((i + 1) % 2), GOAL_HIT_RANGE))
		{
			player2_.player->SetIsGoal(true);
		}
	}

	// 二人が星に触れた状態になったらゴール状態に遷移
	if (player1_.player->GetIsGoal() && player2_.player->GetIsGoal())
	{
		isProcessStop = true;

		ChangeState(GAME_STATE::GOAL);
	}

	return isProcessStop;
}

void GameScene::DrawInfo(void)
{
	if (state_ != GAME_STATE::INFO && state_ != GAME_STATE::INFO3D) { return; }

	int imgNum = curInfoNum_ * 2;
	if (stage_->GetStageType() == StageController::STAGE_TYPE::GRAVITY
		|| stage_->GetStageType() == StageController::STAGE_TYPE::GRAVITY3D)
	{
		imgNum++;
	}
	if (state_ == GAME_STATE::INFO3D)
	{
		imgNum += (INFO_MAX * 2);
	}

	const float SCALE = 0.8f;
	DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, SCALE, 0.0,
				  infoImages_[imgNum], true);
}

void GameScene::ChangeState(GAME_STATE _state)
{
	// BGM減少量
	const float BGM_SOUND_DEC = 0.3f;

	if (_state == GAME_STATE::ACTIVE)
	{
		updateGameStateProc_ = std::bind(&GameScene::Update_Active, this);
		performTime_ = ((state_ == GAME_STATE::PAUSE) ? TIME_PAUSE : TIME_START);

		// BGMの音量を調整
		sound_.SetVolume(static_cast<int>(ResourceManager::SRC::BGM_GAME), sound_.VOLUME_GAME);

		/* 初回のみ自動時間移動 */
		isGameTimeActive_ = (state_ == GAME_STATE::NONE);

		if (state_ == GAME_STATE::INFO)
		{
			// BGMを停止
			sound_.StopAllChoice(true);

			// 初回スタート時、音声を再生
			sound_.Play(static_cast<int>(ResourceManager::SRC::BGM_GAME), true);
			sound_.Play(static_cast<int>(ResourceManager::SRC::SE_COUNT), false);
		}
		else
		{
			sound_.Play(static_cast<int>(ResourceManager::SRC::SE_COUNT_SHORT), false);
		}


		if ((preStageType_ % 2) == 0
			&& static_cast<int>(stage_->GetStageType()) % 2 == 1)
		{
			// ステージが3Dステージに移行した場合、3D説明状態に遷移
			ChangeState(GAME_STATE::INFO3D);
			return;
		}
	}

	else if (_state == GAME_STATE::INFO
		|| _state == GAME_STATE::INFO3D)
	{
		updateGameStateProc_ = std::bind(&GameScene::Update_Info, this);
		curInfoNum_ = 0;

		if (_state == GAME_STATE::INFO3D)
		{
			preStageType_++;
		}

		// BGMの音量を調整
		float volume = sound_.GetVolume(static_cast<int>(ResourceManager::SRC::BGM_GAME)) - BGM_SOUND_DEC;
		sound_.SetVolume(static_cast<int>(ResourceManager::SRC::BGM_GAME), volume);
	}

	else if (_state == GAME_STATE::PAUSE)
	{
		updateGameStateProc_ = std::bind(&GameScene::Update_Pause, this);

		// BGMの音量を調整
		float volume = sound_.GetVolume(static_cast<int>(ResourceManager::SRC::BGM_GAME)) - BGM_SOUND_DEC;
		sound_.SetVolume(static_cast<int>(ResourceManager::SRC::BGM_GAME), volume);
	}

	else if (_state == GAME_STATE::GOAL)
	{
		sound_.Play(static_cast<int>(ResourceManager::SRC::SE_FANFALE), false);
		updateGameStateProc_ = std::bind(&GameScene::Update_Goal, this);
		performTime_ = TIME_CLEAR;

		const float HIT_STOP_GOAL = 1.5f;
		sceneMng_.GetPerform().SetHitStop(HIT_STOP_GOAL);
	}

	else if (_state == GAME_STATE::GAME_CLEAR)
	{
		updateGameStateProc_ = std::bind(&GameScene::Update_Clear, this);
		performTime_ = TIME_GAME_END;

		// BGMの音量を調整
		float volume = sound_.GetVolume(static_cast<int>(ResourceManager::SRC::BGM_GAME)) - BGM_SOUND_DEC;
		sound_.SetVolume(static_cast<int>(ResourceManager::SRC::BGM_GAME), volume);
	}
	else if (_state == GAME_STATE::GAME_OVER)
	{
		updateGameStateProc_ = std::bind(&GameScene::Update_GameOver, this);
		performTime_ = TIME_GAME_END;
	}

	isPerform_ = (performTime_ >= 0.0f);
	state_ = _state;
}

void GameScene::Update_Active(void)
{
	// 移動操作時、ゲーム開始
	if (input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_UP, Input::JOYPAD_NO::PAD1)
		|| input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_DOWN, Input::JOYPAD_NO::PAD1)
		|| input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_LEFT, Input::JOYPAD_NO::PAD1)
		|| input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_RIGHT, Input::JOYPAD_NO::PAD1)
		|| input_.IsTrgDown(InputManager::TYPE::PLAYER_CHANGE, Input::JOYPAD_NO::PAD1)

		|| input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_UP, Input::JOYPAD_NO::PAD2)
		|| input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_DOWN, Input::JOYPAD_NO::PAD2)
		|| input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_LEFT, Input::JOYPAD_NO::PAD2)
		|| input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_RIGHT, Input::JOYPAD_NO::PAD2)
		|| input_.IsTrgDown(InputManager::TYPE::PLAYER_CHANGE, Input::JOYPAD_NO::PAD2))
	{
		isGameTimeActive_ = true;
	}

	// 一時停止状態の切替
	if (input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
		|| input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
	{
		// BGM停止/SE再生
		sound_.Play(static_cast<int>(ResourceManager::SRC::SE_CLICK), false, true);

		ChangeState(GAME_STATE::PAUSE);
	}

	if (stage_->GetIsStageClear()) { return; }

	if (gameTimer_ <= 0.0f)
	{
		ChangeState(GAME_STATE::GAME_OVER);
	}

	if (performTime_ >= -1.0f && isPerform_)
	{
		performTime_ -= sceneMng_.GetDeltaTime();
	}
}
void GameScene::Update_Info(void)
{
	if (input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
		|| input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
	{
		if (--curInfoNum_ < 0
			&& state_ == GAME_STATE::INFO)
		{
			sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
	if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
		|| input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2))
	{
		const int MAX = ((state_ == GAME_STATE::INFO)
			? INFO_MAX
			: INFO3D_MAX);

			if (++curInfoNum_ >= MAX)
			{
				ChangeState(GAME_STATE::ACTIVE);
			}
			else
			{
				sound_.Play(static_cast<int>(ResourceManager::SRC::SE_CLICK), false);
			}
	}
}
void GameScene::Update_Goal(void)
{
	if (performTime_ >= 0.0f && isPerform_)
	{
		performTime_ -= sceneMng_.GetDeltaTime();
	}
	else
	{
		SetStageType();
	}
}
void GameScene::Update_Pause(void)
{
	// 一時停止状態の切替
	if (input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
		|| input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
	{
		// SE再生
		sound_.Play(static_cast<int>(ResourceManager::SRC::SE_CLICK), false, true);

		sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
		|| input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2))
	{
		// BGM・SE再生
		sound_.Play(static_cast<int>(ResourceManager::SRC::SE_CLICK), false, true);
		sound_.Play(static_cast<int>(ResourceManager::SRC::BGM_GAME), true);

		ChangeState(GAME_STATE::ACTIVE);
	}
}
void GameScene::Update_Clear(void)
{
	if (performTime_ > 0.0f)
	{
		performTime_ -= sceneMng_.GetDeltaTime();

		if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
			|| input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
			|| input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2)
			|| input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
		{
			performTime_ = 0.0f;
		}
	}
	else
	{
		if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
			|| input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2))
		{
			isPerform_ = true;
			sound_.Play(static_cast<int>(ResourceManager::SRC::SE_CLICK), false);
			sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		}
		if (input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
			|| input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
		{
			isPerform_ = true;
			sound_.Play(static_cast<int>(ResourceManager::SRC::SE_CLICK), false);
			sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
}
void GameScene::Update_GameOver(void)
{
	if (performTime_ > 0.0f)
	{
		performTime_ -= sceneMng_.GetDeltaTime();

		if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
			|| input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
			|| input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2)
			|| input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
		{
			performTime_ = 0.0f;
		}
	}
	else
	{
		if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
			|| input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2))
		{
			isPerform_ = true;
			sound_.Play(static_cast<int>(ResourceManager::SRC::SE_CLICK), false);
			sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		}
		if (input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
			|| input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
		{
			isPerform_ = true;
			sound_.Play(static_cast<int>(ResourceManager::SRC::SE_CLICK), false);
			sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
}

void GameScene::SetStageType(void)
{
	preStageType_ = static_cast<int>(stage_->GetStageType());

	// 時間を停止
	isGameTimeActive_ = false;

	// ステージ当たり判定を削除
	player1_.player->RemoveHitCollider(ColliderBase::TAG::STAGE);
	player1_.player->RemoveHitCollider(ColliderBase::TAG::GOAL);
	player2_.player->RemoveHitCollider(ColliderBase::TAG::STAGE);
	player2_.player->RemoveHitCollider(ColliderBase::TAG::GOAL);

	// ステージ状態を進める
	stage_->ChangeStages();

	// ステージが終了した場合、以下の処理を終了
	if (stage_->GetIsStageClear())
	{
		ChangeState(GAME_STATE::GAME_CLEAR);
		return;
	}


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
		player1_.player->Update();
		player1_.initialPos = stagePos;
	}
	if (player2_.player != nullptr)
	{
		VECTOR stagePos = stage_->GetPlayerPos(static_cast<int>(Player::PLAYER_NO::P2));
		player2_.player->Init(stagePos, pStageType);
		player2_.player->Update();
		player2_.initialPos = stagePos;
	}

	currentSwapRight_ = SWAP_RIGHT::P1;
	ChangeState(GAME_STATE::ACTIVE);
}

void GameScene::DrawTimer(void)
{
	const float TEXT_SCALE = 0.5f;
	const int TEXT_SIZE = static_cast<int>((80 * TEXT_SCALE));

	int x = Application::SCREEN_HALF_X - 100;
	int arrayNum = 0;

	// 小数点以下の数値
	float frac = (gameTimer_ - std::floor(gameTimer_));


	DrawRotaGraph(x, TEXT_SIZE,
		TEXT_SCALE, 0.0, uiText_[static_cast<int>(UI_TEXT::TIME_LIMIT)], true);

	// 100の位
	x += 150;
	arrayNum = static_cast<int>(gameTimer_ / 100.0f);
	if (arrayNum > 0)
	{
		x += TEXT_SIZE;
		DrawRotaGraph(x, TEXT_SIZE,
			TEXT_SCALE, 0.0, timeText_[arrayNum], true);
	}

	// 10の位
	x += TEXT_SIZE;
	arrayNum = static_cast<int>(gameTimer_ / 10.0f) % 10;
	DrawRotaGraph(x, TEXT_SIZE,
		TEXT_SCALE, 0.0, timeText_[arrayNum], true);

	// 1の位
	x += TEXT_SIZE;
	arrayNum = static_cast<int>(gameTimer_) % 10;
	DrawRotaGraph(x, TEXT_SIZE,
		TEXT_SCALE, 0.0, timeText_[arrayNum], true);

	// 小数点
	x += TEXT_SIZE;
	arrayNum = static_cast<int>(gameTimer_) % 10;
	DrawRotaGraph(x, TEXT_SIZE,
		TEXT_SCALE, 0.0, timeText_[10], true);

	// 第1小数点
	x += TEXT_SIZE;
	arrayNum = static_cast<int>(frac * 10.0f) % 10;
	DrawRotaGraph(x, TEXT_SIZE,
		TEXT_SCALE, 0.0, timeText_[arrayNum], true);

	// 第2小数点
	x += TEXT_SIZE;
	arrayNum = static_cast<int>(frac * 100.0f) % 10;
	DrawRotaGraph(x, TEXT_SIZE,
		TEXT_SCALE, 0.0, timeText_[arrayNum], true);


	// ゲーム開始カウンタ
	if (state_ == GAME_STATE::ACTIVE && performTime_ > -1.0f)
	{
		arrayNum = static_cast<int>(performTime_) + 1;
		int image = ((performTime_ > 0.0f)
			? timeText_[arrayNum]
			: uiText_[static_cast<int>(UI_TEXT::GAME_START)]);

		double scale = ((performTime_ > 0.0f) ? 2.0 : 1.0);

		DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y - 100
			, scale, 0.0, image, true);
	}
}
