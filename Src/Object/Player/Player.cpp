#include "Player.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/MatrixUtility.h"
#include "../Common/AnimationController.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../Collider/ColliderLine.h"
#include "../Collider/ColliderCapsule.h"
#include "../Collider/ColliderModel.h"


Player::Player(PLAYER_NO _playerNo, const VECTOR& _pos)
	:CharaBase::CharaBase(),
	isDash_(false),
	playerNo_(_playerNo),
	stageType_(STAGE_TYPE::MAX)
{
	transform_.pos = _pos;
}

void Player::SetPlayerNo(PLAYER_NO no)
{
	playerNo_ = no;
}

void Player::SetGameStageType(STAGE_TYPE stageType)
{
	if (stageType_ == stageType) { return; }

	stageType_ = stageType;

	// モデル種類変更
	ResourceManager::SRC src = ResourceManager::SRC::MODEL_PLAYER;

	if (playerNo_ == PLAYER_NO::P1)
	{
		if (stageType == STAGE_TYPE::GRAVITY)
		{
			src = ResourceManager::SRC::MODEL_PLAYER_WIDTH;
		}
		else if (stageType == STAGE_TYPE::MOVE)
		{
			src = ResourceManager::SRC::MODEL_PLAYER_WIDTH;
		}
	}
	else if (playerNo_ == PLAYER_NO::P2)
	{
		if (stageType == STAGE_TYPE::GRAVITY)
		{
			src = ResourceManager::SRC::MODEL_PLAYER_HEIGHT;
		}
		else if (stageType == STAGE_TYPE::MOVE)
		{
			src = ResourceManager::SRC::MODEL_PLAYER_HEIGHT;
		}
	}

	int temp = resMng_.LoadModelDuplicate(src);
	// モデル割り当て
	transform_.SetModel(resMng_.LoadModelDuplicate(src));
}

void Player::InitLoadPost(void)
{

}

void Player::Init(const VECTOR& _pos, STAGE_TYPE _stageType)
{
	CharaBase::Init();	

	SetGameStageType(_stageType);
}

void Player::InitTransform(void)
{
	constexpr float MODEL_SCALE = 1.0f;
	float rot = 0.0f;
	if (playerNo_ == PLAYER_NO::P1) 
	{
		rot = 90.0f;
	}


	transform_.InitTransform(MODEL_SCALE,
							 Quaternion::AngleAxis(rot, AsoUtility::AXIS_Y),
							 Quaternion::AngleAxis(180.0f, AsoUtility::AXIS_Y));
}

void Player::InitCollider(void)
{
	// 主に地面との衝突で仕様する線分コライダ
	ColliderLine* colLine = new ColliderLine(ColliderBase::TAG::PLAYER, &transform_,
											 COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);

	// 主に壁や木などの衝突で仕様するカプセルコライダ
	VECTOR colPosTop = AsoUtility::VECTOR_ZERO;
	VECTOR colPosBottom = AsoUtility::VECTOR_ZERO;
	if (playerNo_ == PLAYER_NO::P1)
	{
		colPosTop = COL_CAPSULE_TOP_POS_P1;
		colPosBottom = COL_CAPSULE_DOWN_POS_P1;
	}
	else if (playerNo_ == PLAYER_NO::P2)
	{
		colPosTop = COL_CAPSULE_TOP_POS_P2;
		colPosBottom = COL_CAPSULE_DOWN_POS_P2;
	}
	ColliderCapsule* colCapsule = new ColliderCapsule(ColliderBase::TAG::PLAYER, &transform_,
													  colPosTop, colPosBottom,
													  COL_CAPSULE_RADIUS);

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);


	// 当たり判定リストに格納
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);
}

void Player::InitAnimationPost(void)
{
	PlayAnim(ANIM_TYPE::IDLE);
}

void Player::InitPost(void)
{
}

void Player::UpdateProcess(void)
{
	ProcessJump();

	// 移動操作
	ProcessMove();
}

void Player::UpdateProcessPost(void)
{
	
}

void Player::CollisionReserve(void)
{
	/* アニメーションごとの衝突位置調整 */
	/*
	if (animation_ == nullptr) { return; }
	if (animation_->GetPlayType() == static_cast<int>(ANIM_TYPE::JUMP))
	{
		// ジャンプ中は線分を伸ばす
		if (ownColliders_.count(static_cast<int>(COLLIDER_TYPE::LINE)) != 0)
		{
			ColliderLine* colLine = dynamic_cast<ColliderLine*>(
				ownColliders_.at(static_cast<int>(COLLIDER_TYPE::LINE)));
			colLine->SetLocalPosStart(COL_LINE_JUMP_START_LOCAL_POS);
			colLine->SetLocalPosEnd(COL_LINE_JUMP_END_LOCAL_POS);
		}

		// ジャンプ中はカプセルを伸ばす
		if (ownColliders_.count(static_cast<int>(COLLIDER_TYPE::CAPSULE)) != 0)
		{
			ColliderCapsule* colCapsule = dynamic_cast<ColliderCapsule*>(
				ownColliders_.at(static_cast<int>(COLLIDER_TYPE::CAPSULE)));
			colCapsule->SetLocalPosTop(COL_CAPSULE_TOP_JUMP_LOCAL_POS);
			colCapsule->SetLocalPosDown(COL_CAPSULE_DOWN_JUMP_LOCAL_POS);
		}
	}
	else
	{
		// 通常時の線分に戻す
		if (ownColliders_.count(static_cast<int>(COLLIDER_TYPE::LINE)) != 0)
		{
			ColliderLine* colLine = dynamic_cast<ColliderLine*>(
				ownColliders_.at(static_cast<int>(COLLIDER_TYPE::LINE)));
			colLine->SetLocalPosStart(COL_LINE_START_LOCAL_POS);
			colLine->SetLocalPosEnd(COL_LINE_END_LOCAL_POS);
		}

		// 通常時のカプセルに戻す
		if (ownColliders_.count(static_cast<int>(COLLIDER_TYPE::CAPSULE)) != 0)
		{
			ColliderCapsule* colCapsule = dynamic_cast<ColliderCapsule*>(
				ownColliders_.at(static_cast<int>(COLLIDER_TYPE::CAPSULE)));
			colCapsule->SetLocalPosTop(COL_CAPSULE_TOP_LOCAL_POS);
			colCapsule->SetLocalPosDown(COL_CAPSULE_DOWN_LOCAL_POS);
		}
	}*/
}

void Player::ProcessMove(void)
{
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	if (GetJoypadNum() > 0)
	{
		dir = InputManager::GetInstance().GetDirXY_LStick(Input::JOYPAD_NO::PAD1);
	}
	else
	{
		if (playerNo_ == PLAYER_NO::P1)
		{
			//if (InputManager::GetInstance().IsNew(InputManager::TYPE::PLAYER1_MOVE_BACK))  { dir.y += 1.0f; }
			//if (InputManager::GetInstance().IsNew(InputManager::TYPE::PLAYER1_MOVE_FRONT)) { dir.y -= 1.0f; }
			if (InputManager::GetInstance().IsNew(InputManager::TYPE::PLAYER1_MOVE_LEFT))  { dir.x -= 1.0f; }
			if (InputManager::GetInstance().IsNew(InputManager::TYPE::PLAYER1_MOVE_RIGHT)) { dir.x += 1.0f; }
		}
		else if (playerNo_ == PLAYER_NO::P2)
		{
			if (InputManager::GetInstance().IsNew(InputManager::TYPE::PLAYER2_MOVE_UP))  { dir.y += 1.0f; }
			if (InputManager::GetInstance().IsNew(InputManager::TYPE::PLAYER2_MOVE_DOWN)) { dir.y -= 1.0f; }
			//if (InputManager::GetInstance().IsNew(InputManager::TYPE::PLAYER2_MOVE_LEFT))  { dir.x -= 1.0f; }
			//if (InputManager::GetInstance().IsNew(InputManager::TYPE::PLAYER2_MOVE_RIGHT)) { dir.x += 1.0f; }
		}
	}

	if (!AsoUtility::EqualsVZero(dir))
	{
		//movePow_ = AsoUtility::VECTOR_ZERO;

		// ダッシュ入力時にダッシュ加速度にする
		moveSpeed_ = ((isDash_) ? SPEED_DASH : SPEED_MOVE);

		if (!isJump_)
		{
			if (isDash_)
			{
				PlayAnim(ANIM_TYPE::FAST_RUN);
			}
			else
			{
				PlayAnim(ANIM_TYPE::RUN);
			}
		}

		// カメラの方向で進行
		Quaternion cameraRot = sceneMng_.GetCamera()->GetQuaRotY();

		// 移動方向を取得
		moveDir_ = Quaternion::PosAxis(cameraRot, dir);

		// 加速度に割り当て
		movePow_ = VScale(moveDir_, moveSpeed_);
	}
	else
	{
		movePow_ = AsoUtility::VECTOR_ZERO;

		if (!isJump_)
		{
			PlayAnim(ANIM_TYPE::IDLE);
		}
	}
}

void Player::ProcessJump(void)
{
	
}

void Player::PlayAnim(Player::ANIM_TYPE _type, bool _isLoop)
{
	int type = static_cast<int>(_type);

	// 指定したアニメーションが割り当てられているとき、処理終了
	if (animation_ == nullptr || type == animation_->GetPlayType()) return;

	animation_->Play(type, _isLoop);
}