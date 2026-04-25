#include "Player.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/UtilityMath.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/UtilityMatrix.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../Collider/ColliderLine.h"
#include "../Collider/ColliderCapsule.h"
#include "../Collider/ColliderModel.h"


Player::Player(PLAYER_NO _playerNo, const VECTOR& _pos, STAGE_TYPE _stageType)
	: CharaBase::CharaBase(_stageType)
	, input_(InputManager::GetInstance())
	, playerNo_(_playerNo)
	, frameEyeDamage_(-1), frameEyeDefault_(-1)
	, isChangeModel_(false), isHasAuthority_(false)
	, playerState_(STATE::NONE), arrowHandle_(-1)
	, initialPos_(_pos)
	, easingPosStart_(UtilityMath::VECTOR_ZERO),easingPosEnd_(UtilityMath::VECTOR_ZERO)
{
	transform_.pos = _pos;
}

void Player::SetPlayerNo(PLAYER_NO no)
{
	playerNo_ = no;
}

void Player::SetGameStageType(void)
{
	float modelScale = 1.0f;

	// モデル種類変更
	ResourceManager::SRC src = ResourceManager::SRC::NONE;

	if (curStageType_ == STAGE_TYPE::MOVE)
	{
		src = ResourceManager::SRC::MODEL_PLAYER_MOVE;
	}
	
	else if (curStageType_ == STAGE_TYPE::GRAVITY)
	{
		src = ResourceManager::SRC::MODEL_PLAYER_GRAVITY;
		modelScale = 0.75f;

		if (playerNo_ == PLAYER_NO::P1)
		{
			curGravityDir_ = GRAVITY_DIR::RIGHT;

			const Quaternion START_ROT = Quaternion::Mult(
				Quaternion::AngleAxis(90.0f, UtilityMath::AXIS_Z)
				, Quaternion::AngleAxis(90.0f, UtilityMath::AXIS_X)
			);
			transform_.quaRot = START_ROT;
		}
		else if (playerNo_ == PLAYER_NO::P2)
		{
			curGravityDir_ = GRAVITY_DIR::DOWN;
			//transform_.quaRot = Quaternion::Identity();

			const Quaternion START_ROT = Quaternion::Mult(
				Quaternion::AngleAxis(0.0f, UtilityMath::AXIS_Z)
				, Quaternion::AngleAxis(0.0f, UtilityMath::AXIS_Z));
			transform_.quaRot = START_ROT;
		}
	}

	// モデル割り当て
	transform_.SetScale(modelScale);
	transform_.SetModel(resMng_.LoadModelDuplicate(src));


	/* プレイヤー別のマテリアル割り当て */
	COLOR_F matCol = ((playerNo_ == PLAYER_NO::P1) ? P1_COLOR : P2_COLOR);
	MV1SetMaterialDifColor(transform_.modelId, 0, matCol);
	MV1SetMaterialEmiColor(transform_.modelId, 0, GetColorF(0.0f, 0.0f, 0.0f, 0.0f));
	MV1SetMaterialSpcColor(transform_.modelId, 0, GetColorF(0.0f, 0.0f, 0.0f, 0.0f));


	// 目のマテリアルの色を明示的に黒にする
	const int EYE_BLACK_MATERIAL_NUM = ((curStageType_ == STAGE_TYPE::MOVE) ? 2 : 1);

	MV1SetMaterialDifColor(transform_.modelId, EYE_BLACK_MATERIAL_NUM, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	MV1SetMaterialEmiColor(transform_.modelId, EYE_BLACK_MATERIAL_NUM, GetColorF(0.0f, 0.0f, 0.0f, 0.0f));
	MV1SetMaterialSpcColor(transform_.modelId, EYE_BLACK_MATERIAL_NUM, GetColorF(0.0f, 0.0f, 0.0f, 0.0f));
	


	/* モデルの部分非表示 */
	const std::string FRAME_NAME_DAMAGE = "Damage";
	const std::string FRAME_NAME_EYE = "Eye";

	std::string frameName = "";
	bool isVisible = true;

	for (int i = 0; i < MV1GetFrameNum(transform_.modelId); i++)
	{
		frameName = MV1GetFrameName(transform_.modelId, i);

		// ダメージ目を非表示
		if (frameName.find(FRAME_NAME_DAMAGE) != std::string::npos)
		{
			isVisible = false;

			if (curStageType_ == STAGE_TYPE::MOVE) 
			{
				if (playerNo_ == PLAYER_NO::P1
					&& frameName.find("Width") != std::string::npos
					|| playerNo_ == PLAYER_NO::P2
					&& frameName.find("Height") != std::string::npos)
				{
					frameEyeDamage_ = i;
				}
			}
			else
			{
				frameEyeDamage_ = i;
			}
		}
		else if (frameName.find(FRAME_NAME_EYE) != std::string::npos)
		{
			 if (curStageType_ == STAGE_TYPE::MOVE)
			 {
				 isVisible = false;

 				 if (playerNo_ == PLAYER_NO::P1
					 && frameName.find("Width") != std::string::npos
				 	 || playerNo_ == PLAYER_NO::P2
					 && frameName.find("Height") != std::string::npos)
				 {
				 	 isVisible = true;
					 frameEyeDefault_ = i;
				 }
			 }
			 else
			 {
				 isVisible = true;

				 if (frameName.find("1") != std::string::npos)
				 {
					 frameEyeDefault_ = i;
				 }
			 }
		}

		MV1SetFrameVisible(transform_.modelId, i, isVisible);
	}
}

void Player::SetIsChangeModel(bool _isChangeModel)
{
	isChangeModel_ = _isChangeModel;
	Update();
}


void Player::UpdateGravityRotation(void)
{
	if (curGravityDir_ == GRAVITY_DIR::NONE) { return; }

	float rotZ = 0.0f;

	if (playerNo_ == PLAYER_NO::P1)
	{
		// Player1回転
		switch (curGravityDir_)
		{
			case GRAVITY_DIR::UP:    rotZ = 180.0f;  break;
			case GRAVITY_DIR::DOWN:  rotZ = 0.0f;   break;
			case GRAVITY_DIR::LEFT:  rotZ = 90.0f;  break;
			case GRAVITY_DIR::RIGHT: rotZ = -90.0f;    break;

			default: return;
		}

		// 合成：P1は横(90度)を向いた状態で、画面のZ軸を中心に回転
		const Quaternion qBase = Quaternion::AngleAxis(90.0f, UtilityMath::AXIS_X);
		Quaternion qGrav = Quaternion::AngleAxis(rotZ, UtilityMath::AXIS_Z);
		transform_.quaRot = qGrav.Mult(qBase);
	}
	else
	{
		// Player2回転
		switch (curGravityDir_)
		{
			case GRAVITY_DIR::UP:    rotZ = 0.0f;  break;
			case GRAVITY_DIR::DOWN:  rotZ = 180.0f;    break;
			case GRAVITY_DIR::LEFT:  rotZ = -90.0f;  break; 
			case GRAVITY_DIR::RIGHT: rotZ = 90.0f;   break;

			default: return;
		}

		Quaternion qGrav = Quaternion::AngleAxis(rotZ, UtilityMath::AXIS_Z);
		transform_.quaRot = qGrav; // P2は正面向きなのでqBaseなしでOK
	}
}

void Player::SetEasingActive(const VECTOR& _endPos)
{
	/* イージング補間移動状態に遷移 */
	playerState_ = STATE::SWAP;
	easingPosStart_ = transform_.pos;
	easingPosEnd_ = _endPos;
}

void Player::SetEasingPos(float _easingNum)
{
	transform_.pos.x = (easingPosStart_.x + (easingPosEnd_.x - easingPosStart_.x) * _easingNum);
	transform_.pos.y = (easingPosStart_.y + (easingPosEnd_.y - easingPosStart_.y) * _easingNum);
	transform_.pos.z = (easingPosStart_.z + (easingPosEnd_.z - easingPosStart_.z) * _easingNum);

	transform_.prePos = transform_.pos;
}

void Player::SetHitTrap(void)
{
	SetEasingActive(initialPos_);
}

void Player::InitLoadPost(void)
{

}

void Player::ReInit(const VECTOR& _pos, STAGE_TYPE _stageType)
{
	/* 再初期化処理 */
	transform_.pos = initialPos_ = _pos;
	curStageType_ = _stageType;

	CharaBase::Init();
	Update();
}

void Player::InitPost(void)
{
	playerState_ = STATE::ACTIVE;

	isHasAuthority_ = false;
	arrowHandle_ = resMng_.LoadHandleId(ResourceManager::SRC::IMG_PLAYER_ARROW);

	isChangeModel_ = false;

	curGravityDir_ = GRAVITY_DIR::NONE;
	movePow_ = UtilityMath::VECTOR_ZERO;	
	SetGameStageType();
}

void Player::Draw(void)
{
	// ライティングを無効化
	SetUseLighting(FALSE);

	ActorBase::Draw();


	//矢印の描画処理
	if (isHasAuthority_ && arrowHandle_ != -1)
	{
		// Zバッファ設定（壁を透かす）
		SetUseZBuffer3D(FALSE);
		SetWriteZBuffer3D(FALSE);


		// 矢印を描画
		const VECTOR ARROW_OFFSET = VGet(0, 130.0f, 0);
		const int ARROW_SIZE = 175;
		DrawBillboard3D(VAdd(transform_.pos, ARROW_OFFSET)
						, 0.5f, 0.5f, ARROW_SIZE, 0.0f, arrowHandle_, TRUE);

		// 設定を元に戻す
		SetUseZBuffer3D(TRUE);
		SetWriteZBuffer3D(TRUE);
	}

	// ライティングを有効化
	SetUseLighting(TRUE);
}


void Player::InitTransform(void)
{
	//constexpr float MODEL_SCALE = 1.0f;
	//float rot = 0.0f;
	//if (playerNo_ == PLAYER_NO::P1) 
	//{
	//	rot = 90.0f;
	//}


	//transform_.InitTransform(MODEL_SCALE,
	//						 Quaternion::AngleAxis(rot, UtilityMath::AXIS_Y),
	//						 Quaternion::AngleAxis(180.0f, UtilityMath::AXIS_Y));

	constexpr float MODEL_SCALE = 1.0f;
	float rotY = (playerNo_ == PLAYER_NO::P1) ? 90.0f : 0.0f;
	VECTOR localRot = UtilityMath::VECTOR_ZERO;
	localRot.x = ((playerNo_ == PLAYER_NO::P1) ? -90.0f : 0.0f);
	localRot.y = ((playerNo_ == PLAYER_NO::P1) ? 0.0f : 0.0f);
	localRot.z = ((playerNo_ == PLAYER_NO::P1) ? 180.0f : 0.0f);

	Quaternion rotLocal = Quaternion::Mult(Quaternion::AngleAxis(localRot.x, UtilityMath::AXIS_X),
										   Quaternion::AngleAxis(localRot.y, UtilityMath::AXIS_Y));

	// P2かつ重力モードならZ軸で180度回転（逆さま）
	if (playerNo_ == PLAYER_NO::P2
		&& curStageType_ == STAGE_TYPE::GRAVITY)
	{
		localRot.z = 180.0f;
	}
	
	transform_.InitTransform(MODEL_SCALE,
		Quaternion::AngleAxis(rotY, UtilityMath::AXIS_Y),
		rotLocal.Mult(Quaternion::AngleAxis(localRot.z, UtilityMath::AXIS_Z)));

}

void Player::InitCollider(void)
{
	// P2かつ重力モードなら判定を上に向ける
	VECTOR lineStart = COL_LINE_START_LOCAL_POS;
	VECTOR lineEnd = COL_LINE_END_LOCAL_POS;

	if (playerNo_ == PLAYER_NO::P2
		&& curStageType_ == STAGE_TYPE::GRAVITY)
	{
		lineStart = VGet(0, 0, 0);
		lineEnd = VGet(0, 80.0f, 0);
	}

	// 主に地面との衝突で仕様する線分コライダ
	ColliderLine* colLine = new ColliderLine(ColliderBase::TAG::PLAYER, &transform_,
											 COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);

	
	VECTOR colPosTop = ((playerNo_ == PLAYER_NO::P1)
							? COL_CAPSULE_TOP_POS_P1
							: COL_CAPSULE_TOP_POS_P2);

	VECTOR colPosBottom = ((playerNo_ == PLAYER_NO::P1)
							? COL_CAPSULE_DOWN_POS_P1
							: COL_CAPSULE_DOWN_POS_P2);
	
	ColliderCapsule* colCapsule = new ColliderCapsule(ColliderBase::TAG::PLAYER, &transform_,
													  colPosTop, colPosBottom,
													  COL_CAPSULE_RADIUS);

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);


	// 当たり判定リストに格納
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);
}

void Player::UpdateProcess(void)
{
	if (playerState_ == STATE::SWAP)
	{
		return;
	}

	// 移動操作
	ProcessMove();

	// 重力ステージ時、モデルの向きを更新
	if (curStageType_ == STAGE_TYPE::GRAVITY)
	{
		UpdateGravityRotation();
	}
}

void Player::UpdateProcessPost(void)
{
	if (frameEyeDamage_ == -1 || frameEyeDefault_ == -1) { return; }

	/* モデルの見た目を変更する */
	if (isChangeModel_)
	{
		if (MV1GetFrameVisible(transform_.modelId, frameEyeDamage_)) { return; }
		
		// ダメージ目有効化
		MV1SetFrameVisible(transform_.modelId, frameEyeDamage_, true);

		// 通常目無効化
		MV1SetFrameVisible(transform_.modelId, frameEyeDefault_, false);
	}
	else if (!MV1GetFrameVisible(transform_.modelId, frameEyeDefault_))
	{
		// 通常目有効化
		MV1SetFrameVisible(transform_.modelId, frameEyeDefault_, true);

		// ダメージ目無効化
		MV1SetFrameVisible(transform_.modelId, frameEyeDamage_, false);
	}
}

void Player::ProcessMove(void)
{
	// 移動ステージ
	if (curStageType_ == STAGE_TYPE::MOVE)
	{
		VECTOR dir = UtilityMath::VECTOR_ZERO;

		if (playerNo_ == PLAYER_NO::P1)
		{
			if (input_.IsNew(InputManager::TYPE::PLAYER1_MOVE_LEFT, Input::JOYPAD_NO::PAD1)) { dir.x -= 1.0f; }
			if (input_.IsNew(InputManager::TYPE::PLAYER1_MOVE_RIGHT, Input::JOYPAD_NO::PAD1)) { dir.x += 1.0f; }
		}
		else if (playerNo_ == PLAYER_NO::P2)
		{
			// P2のMOVEステージ時の移動（上下操作）
			if (input_.IsNew(InputManager::TYPE::PLAYER2_MOVE_UP, Input::JOYPAD_NO::PAD2)) { dir.y += 1.0f; }
			if (input_.IsNew(InputManager::TYPE::PLAYER2_MOVE_DOWN, Input::JOYPAD_NO::PAD2)) { dir.y -= 1.0f; }
		}

		if (!UtilityMath::EqualsVZero(dir))
		{
			Quaternion cameraRot = sceneMng_.GetCamera()->GetQuaRotY();
			moveDir_ = Quaternion::PosAxis(cameraRot, dir);
			movePow_ = VScale(moveDir_, SPEED_MOVE);
		}
		else
		{
			movePow_ = UtilityMath::VECTOR_ZERO;
		}

		return;
	}

	// 重力ステージ
	else if (curStageType_ == STAGE_TYPE::GRAVITY)
	{
		// 入力による重力方向の切り替え（他方のプレイヤーには影響しない）
		if (playerNo_ == PLAYER_NO::P1)
		{
			if (input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_LEFT, Input::JOYPAD_NO::PAD1))
				{ curGravityDir_ = GRAVITY_DIR::LEFT; }

			if (input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_RIGHT, Input::JOYPAD_NO::PAD1))
				{ curGravityDir_ = GRAVITY_DIR::RIGHT; }
		}
		else if (playerNo_ == PLAYER_NO::P2)
		{
			if (input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_UP, Input::JOYPAD_NO::PAD2))
				{ curGravityDir_ = GRAVITY_DIR::UP; }

			if (input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_DOWN, Input::JOYPAD_NO::PAD2))
				{ curGravityDir_ = GRAVITY_DIR::DOWN; }
		}

		// 物理計算：現在の方向に基づいて加速
		VECTOR accel = UtilityMath::VECTOR_ZERO;
		switch (curGravityDir_)
		{
			case GRAVITY_DIR::UP:    accel.y =  GRAVITY_ACCEL; break;
			case GRAVITY_DIR::DOWN:  accel.y = -GRAVITY_ACCEL; break;
			case GRAVITY_DIR::LEFT:  accel.x = -GRAVITY_ACCEL; break;
			case GRAVITY_DIR::RIGHT: accel.x =  GRAVITY_ACCEL; break;
			default: break;
		}

		// movePow_ に加速度を足し続ける（速度が蓄積される）
		movePow_ = VAdd(movePow_, accel);

		// 最高速度制限
		if (VSize(movePow_) > TERMINAL_VELOCITY)
		{
			movePow_ = VScale(VNorm(movePow_), TERMINAL_VELOCITY);
		}
	}
}

void Player::ChangeState(STATE _state)
{
	playerState_ = _state;

	if (_state == STATE::ACTIVE)
	{
		isChangeModel_ = false;
	}
}
