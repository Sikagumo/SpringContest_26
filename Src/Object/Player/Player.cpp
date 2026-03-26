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
	input_(InputManager::GetInstance()),
	playerNo_(_playerNo),
	stageType_(STAGE_TYPE::MAX),
	frameEyeDamage_(-1), frameEyeDefault_(-1),
	isChangeModel_(false)
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

	float modelScale = 1.0f;

	stageType_ = stageType;

	// モデル種類変更
	ResourceManager::SRC src = ResourceManager::SRC::MODEL_PLAYER;

	if (stageType == STAGE_TYPE::MOVE)
	{
		src = ResourceManager::SRC::MODEL_PLAYER_MOVE;
	}
	else if (stageType == STAGE_TYPE::GRAVITY)
	{
		src = ResourceManager::SRC::MODEL_PLAYER_GRAVITY;
		modelScale = 0.75f;
	}

	// モデル割り当て
	transform_.SetScale(modelScale);
	transform_.SetModel(resMng_.LoadModelDuplicate(src));

	// プレイヤー別のマテリアル割り当て
	COLOR_F matCol = MV1GetMaterialDifColor(transform_.modelId, 0);
	matCol = ((playerNo_ == PLAYER_NO::P1) ? COLOR_F(1.0f, 0.25f, 0.25f, 1.0f) : COLOR_F(0.25f, 0.25f, 1.0f, 1.0f));
	MV1SetMaterialDifColor(transform_.modelId, 0, matCol);

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

			if (stageType_ == STAGE_TYPE::MOVE)
			{
				if (playerNo_ == PLAYER_NO::P1
					&& frameName.find("Width") != std::string::npos
					|| playerNo_ == PLAYER_NO::P2
					&& frameName.find("Height") != std::string::npos)
				{
					isVisible = ((isChangeModel_) ? true : false);
					frameEyeDamage_ = i;
				}
			}
		}
		else if (frameName.find(FRAME_NAME_EYE) != std::string::npos)
		{
			if (stageType_ == STAGE_TYPE::MOVE)
			{
				isVisible = false;

				if (playerNo_ == PLAYER_NO::P1
					&& frameName.find("Width") != std::string::npos
					|| playerNo_ == PLAYER_NO::P2
					&& frameName.find("Height") != std::string::npos)
				{
					isVisible = ((isChangeModel_) ? false : true);
					frameEyeDefault_ = i;
				}
			}
		}

		MV1SetFrameVisible(transform_.modelId, i, isVisible);
	}
}

void Player::InitLoadPost(void)
{

}

void Player::Init(const VECTOR& _pos, STAGE_TYPE _stageType)
{
	CharaBase::Init();

	isChangeModel_ = false;

	transform_.pos = _pos;
	SetGameStageType(_stageType);

	const float LIGHT_RANGE = 100.0f;
	lightHandle_ = CreatePointLightHandle(transform_.pos, LIGHT_RANGE, 0.0f, 0.001f, 0.0f);

	SetLightTypeHandle(lightHandle_, DX_LIGHTTYPE_POINT);
}

void Player::Draw(void)
{
	// ライティングを無効化
	SetUseLighting(FALSE);
	//SetUseZBuffer3D(FALSE);

	ActorBase::Draw();

	// ライティングを無効化
	SetUseLighting(TRUE);
	//SetUseZBuffer3D(TRUE);

	const float LIGHT_POS_Z = 0.0f;
	VECTOR lightPos = VAdd(transform_.pos, VGet(0.0f, 0.0f, LIGHT_POS_Z));
	SetLightPositionHandle(lightHandle_, lightPos);
}

void Player::Release(void)
{
	ActorBase::Release();

	DeleteLightHandle(lightHandle_);
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
	//						 Quaternion::AngleAxis(rot, AsoUtility::AXIS_Y),
	//						 Quaternion::AngleAxis(180.0f, AsoUtility::AXIS_Y));

	constexpr float MODEL_SCALE = 1.0f;
	float rotY = (playerNo_ == PLAYER_NO::P1) ? 90.0f : 0.0f;
	float rotZ = 0.0f;

	// P2かつ重力モードならZ軸で180度回転（逆さま）
	if (playerNo_ == PLAYER_NO::P2 && stageType_ == STAGE_TYPE::GRAVITY)
	{
		rotZ = 180.0f;
	}

	transform_.InitTransform(MODEL_SCALE,
		Quaternion::AngleAxis(rotY, AsoUtility::AXIS_Y),
		Quaternion::AngleAxis(rotZ, AsoUtility::AXIS_Z)); // 180度反転

}

void Player::InitCollider(void)
{
	//変更箇所：P2かつ重力モードなら判定を上に向ける
	VECTOR lineStart = COL_LINE_START_LOCAL_POS;
	VECTOR lineEnd = COL_LINE_END_LOCAL_POS;

	if (playerNo_ == PLAYER_NO::P2 && stageType_ == STAGE_TYPE::GRAVITY)
	{
		lineStart = VGet(0, 0, 0);
		lineEnd = VGet(0, 80.0f, 0); // 上向きに判定を出す
	}

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

	if (playerNo_ == PLAYER_NO::P1)
	{
		//if (input_.IsNew(InputManager::TYPE::PLAYER1_MOVE_BACK))  { dir.y += 1.0f; }
		//if (input_.IsNew(InputManager::TYPE::PLAYER1_MOVE_FRONT)) { dir.y -= 1.0f; }
		if (input_.IsNew(InputManager::TYPE::PLAYER1_MOVE_LEFT, Input::JOYPAD_NO::PAD1))
		{
			dir.x -= 1.0f;
		}
		if (input_.IsNew(InputManager::TYPE::PLAYER1_MOVE_RIGHT, Input::JOYPAD_NO::PAD1))
		{
			dir.x += 1.0f;
		}
	}
	else if (playerNo_ == PLAYER_NO::P2)
	{
		// --- 修正箇所：重力モードなら左右、そうでなければ元の上下操作 ---
		if (stageType_ == STAGE_TYPE::GRAVITY)
		{
			if (input_.IsNew(InputManager::TYPE::PLAYER2_MOVE_LEFT, Input::JOYPAD_NO::PAD2))
			{
				dir.x -= 1.0f;
			}
			if (input_.IsNew(InputManager::TYPE::PLAYER2_MOVE_RIGHT, Input::JOYPAD_NO::PAD2))
			{
				dir.x += 1.0f;
			}
		}
		else
		{
			if (input_.IsNew(InputManager::TYPE::PLAYER2_MOVE_UP, Input::JOYPAD_NO::PAD2))
			{
				dir.y += 1.0f;
			}
			if (input_.IsNew(InputManager::TYPE::PLAYER2_MOVE_DOWN, Input::JOYPAD_NO::PAD2))
			{
				dir.y -= 1.0f;
			}
		}
	}

	if (GetJoypadNum() > 0)
	{
		//dir = input_.GetDirXY_LStick(Input::JOYPAD_NO::PAD1);
	}
	else
	{
		
	}

	if (!AsoUtility::EqualsVZero(dir))
	{
		//movePow_ = AsoUtility::VECTOR_ZERO;

		// ダッシュ入力時にダッシュ加速度にする
		moveSpeed_ = SPEED_MOVE;

		if (!isJump_)
		{
			PlayAnim(ANIM_TYPE::RUN);
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

	// 重力の適用
	if (stageType_ == STAGE_TYPE::GRAVITY)
	{
		const float GRAVITY_POW = 9.8f;
		if (playerNo_ == PLAYER_NO::P1)
		{
			//下重力 
			movePow_.y -= GRAVITY_POW;
		}
		else if (playerNo_ == PLAYER_NO::P2)
		{
			//上重力
			movePow_.y += GRAVITY_POW;
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

