#include "Player.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/MatrixUtility.h"
#include "../Common/AnimationController.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../Collider/ColliderLine.h"
#include "../Collider/ColliderCapsule.h"
#include "../Collider/ColliderModel.h"


Player::Player(PLAYER_NO _playerNo, const VECTOR& _pos)
	: CharaBase::CharaBase()
	, input_(InputManager::GetInstance())
	, playerNo_(_playerNo)
	, frameEyeDamage_(-1), frameEyeDefault_(-1)
	, isChangeModel_(false), hasAuthority_(false)
	, isGoal_(false), arrowHandle_(-1)
	, lightHandle_(-1)
{
	stageType_ = static_cast<int>(STAGE_TYPE::MAX);

	transform_.pos = _pos;
}

void Player::SetPlayerNo(PLAYER_NO no)
{
	playerNo_ = no;
}

void Player::SetGameStageType(STAGE_TYPE stageType)
{
	if (stageType_ == static_cast<int>(stageType)) { return; }

	float modelScale = 1.0f;

	stageType_ = static_cast<int>(stageType);

	// モデル種類変更
	ResourceManager::SRC src = ResourceManager::SRC::NONE;

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


	/* プレイヤー別のマテリアル割り当て */
	const float DEC_NUM = 0.75;
	COLOR_F matCol = ((playerNo_ == PLAYER_NO::P1) ? P1_COLOR : P2_COLOR);
	MV1SetMaterialDifColor(transform_.modelId, 0, matCol);


	// 目のマテリアルの色を明示的に黒にする
	const int EYE_BLACK_MATERIAL_NUM = ((stageType_ == static_cast<int>(STAGE_TYPE::MOVE)) ? 2 : 1);

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

			if (stageType_ == static_cast<int>(STAGE_TYPE::MOVE)) 
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
			if (stageType_ == static_cast<int>(STAGE_TYPE::GRAVITY)) 
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
		// --- Player 1 専用：現在の「ズレ」を強制修正する数値 ---
		switch (curGravityDir_)
		{
		case GRAVITY_DIR::UP:    rotZ = -90.0f;  break; // 「上」で左を向くなら、-90度して上に合わせる
		case GRAVITY_DIR::DOWN:  rotZ = 90.0f;   break; // 「下」で右を向くなら、90度して下に合わせる
		case GRAVITY_DIR::LEFT:  rotZ = 180.0f;  break; // 左右逆転を直す
		case GRAVITY_DIR::RIGHT: rotZ = 0.0f;    break; // 左右逆転を直す
		default: return;
		}

		// 合成：P1は横(90度)を向いた状態で、画面のZ軸を中心に回転
		Quaternion qBase = Quaternion::AngleAxis(90.0f, AsoUtility::AXIS_Y);
		Quaternion qGrav = Quaternion::AngleAxis(rotZ, AsoUtility::AXIS_Z);
		transform_.quaRot = qGrav.Mult(qBase);
	}
	else
	{
		// --- Player 2 専用：標準的な回転 (P2がおかしければここを調整) ---
		switch (curGravityDir_)
		{
		case GRAVITY_DIR::UP:    rotZ = 180.0f;  break;
		case GRAVITY_DIR::DOWN:  rotZ = 0.0f;    break;
		case GRAVITY_DIR::LEFT:  rotZ = -90.0f;  break; 
		case GRAVITY_DIR::RIGHT: rotZ = 90.0f;   break;
		default: return;
		}
		Quaternion qGrav = Quaternion::AngleAxis(rotZ, AsoUtility::AXIS_Z);
		transform_.quaRot = qGrav; // P2は正面向きなのでqBaseなしでOK
	}
}

void Player::InitLoadPost(void)
{

}

void Player::Init(const VECTOR& _pos, STAGE_TYPE _stageType)
{
	CharaBase::Init();

	isGoal_ = false;
	
	arrowHandle_ = resMng_.LoadHandleId(ResourceManager::SRC::IMG_PLAYER_ARROW);

	isChangeModel_ = false;

	transform_.pos = _pos;
	SetGameStageType(_stageType);

	const float LIGHT_RANGE = 100.0f;
	lightHandle_ = CreatePointLightHandle(transform_.pos, LIGHT_RANGE, 0.0f, 0.001f, 0.0f);

	SetLightTypeHandle(lightHandle_, DX_LIGHTTYPE_POINT);

	curGravityDir_ = GRAVITY_DIR::NONE;
	movePow_ = AsoUtility::VECTOR_ZERO;
}

void Player::Draw(void)
{
	// ライティングを無効化
	SetUseLighting(FALSE);

	ActorBase::Draw();


	//矢印の描画処理
	if (hasAuthority_ && arrowHandle_ != -1)
	{
		// Zバッファ設定（壁を透かす）
		SetUseZBuffer3D(FALSE);
		SetWriteZBuffer3D(FALSE);


		// 矢印を描画
		const VECTOR ARROW_OFFSET = VGet(0, 130.0f, 0);
		const float ARROW_SIZE = 175.0f;
		DrawBillboard3D(VAdd(transform_.pos, ARROW_OFFSET)
						, 0.5f, 0.5f, static_cast<int>(ARROW_SIZE), 0.0f, arrowHandle_, TRUE);

		// 設定を元に戻す
		SetUseZBuffer3D(TRUE);
		SetWriteZBuffer3D(TRUE);
	}

	// ライティングを有効化
	SetUseLighting(TRUE);

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
	float localRotX = ((playerNo_ == PLAYER_NO::P1) ? -90.0f : 0.0f);
	float localRotY = ((playerNo_ == PLAYER_NO::P1) ? 0.0f : 0.0f);
	float localRotZ = ((playerNo_ == PLAYER_NO::P1) ? 180.0f : 0.0f);

	Quaternion rotLocal = Quaternion::Mult(Quaternion::AngleAxis(localRotX, AsoUtility::AXIS_X),
										   Quaternion::AngleAxis(localRotY, AsoUtility::AXIS_Y));

	// P2かつ重力モードならZ軸で180度回転（逆さま）
	if (playerNo_ == PLAYER_NO::P2 && stageType_ == static_cast<int>(STAGE_TYPE::GRAVITY)) 
	{
		localRotZ = 180.0f;
	}
	
	transform_.InitTransform(MODEL_SCALE,
		Quaternion::AngleAxis(rotY, AsoUtility::AXIS_Y),
		rotLocal.Mult(Quaternion::AngleAxis(localRotZ, AsoUtility::AXIS_Z))); // 180度反転

}

void Player::InitCollider(void)
{
	//変更箇所：P2かつ重力モードなら判定を上に向ける
	VECTOR lineStart = COL_LINE_START_LOCAL_POS;
	VECTOR lineEnd = COL_LINE_END_LOCAL_POS;

	if (playerNo_ == PLAYER_NO::P2 && stageType_ == static_cast<int>(STAGE_TYPE::GRAVITY)) 
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

	// 重力ステージならモデルの向きを更新
	if (stageType_ == static_cast<int>(STAGE_TYPE::GRAVITY))
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

//void Player::ProcessMove(void)
//{
//	VECTOR dir = AsoUtility::VECTOR_ZERO;
//
//	if (playerNo_ == PLAYER_NO::P1)
//	{
//		//if (input_.IsNew(InputManager::TYPE::PLAYER1_MOVE_BACK))  { dir.y += 1.0f; }
//		//if (input_.IsNew(InputManager::TYPE::PLAYER1_MOVE_FRONT)) { dir.y -= 1.0f; }
//		if (input_.IsNew(InputManager::TYPE::PLAYER1_MOVE_LEFT, Input::JOYPAD_NO::PAD1))
//		{
//			dir.x -= 1.0f;
//		}
//		if (input_.IsNew(InputManager::TYPE::PLAYER1_MOVE_RIGHT, Input::JOYPAD_NO::PAD1))
//		{
//			dir.x += 1.0f;
//		}
//	}
//	else if (playerNo_ == PLAYER_NO::P2)
//	{
//		if (stageType_ == STAGE_TYPE::GRAVITY)
//		{
//			if (input_.IsNew(InputManager::TYPE::PLAYER2_MOVE_LEFT, Input::JOYPAD_NO::PAD2))
//			{
//				dir.x -= 1.0f;
//			}
//			if (input_.IsNew(InputManager::TYPE::PLAYER2_MOVE_RIGHT, Input::JOYPAD_NO::PAD2))
//			{
//				dir.x += 1.0f;
//			}
//		}
//		else
//		{
//			if (input_.IsNew(InputManager::TYPE::PLAYER2_MOVE_UP, Input::JOYPAD_NO::PAD2))
//			{
//				dir.y += 1.0f;
//			}
//			if (input_.IsNew(InputManager::TYPE::PLAYER2_MOVE_DOWN, Input::JOYPAD_NO::PAD2))
//			{
//				dir.y -= 1.0f;
//			}
//		}
//	}
//
//	if (!AsoUtility::EqualsVZero(dir))
//	{
//		// ダッシュ入力時にダッシュ加速度にする
//		moveSpeed_ = SPEED_MOVE;
//
//		if (!isJump_)
//		{
//			PlayAnim(ANIM_TYPE::RUN);
//		}
//
//
//		// カメラの方向で進行
//		Quaternion cameraRot = sceneMng_.GetCamera()->GetQuaRotY();
//
//		// 移動方向を取得
//		moveDir_ = Quaternion::PosAxis(cameraRot, dir);
//
//		// 加速度に割り当て
//		movePow_ = VScale(moveDir_, moveSpeed_);
//	}
//	else
//	{
//		movePow_ = AsoUtility::VECTOR_ZERO;
//
//		if (!isJump_)
//		{
//			PlayAnim(ANIM_TYPE::IDLE);
//		}
//	}
//
//	// 重力の適用
//	if (stageType_ == STAGE_TYPE::GRAVITY) {
//
//		// 1. プレイヤーごとの入力分離
//		if (playerNo_ == PLAYER_NO::P1) {
//			// Player1 は W/S (PAD1 系統) のみを受け付ける
//			if (input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_UP, Input::JOYPAD_NO::PAD1)) {
//				curGravityDir_ = GRAVITY_DIR::UP;
//			}
//			if (input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_DOWN, Input::JOYPAD_NO::PAD1)) {
//				curGravityDir_ = GRAVITY_DIR::DOWN;
//			}
//		}
//		else if (playerNo_ == PLAYER_NO::P2) {
//			// Player2 は 左右矢印 (PAD2 系統) のみを受け付ける
//			if (input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_LEFT, Input::JOYPAD_NO::PAD2)) {
//				curGravityDir_ = GRAVITY_DIR::LEFT;
//			}
//			if (input_.IsTrgDown(InputManager::TYPE::PLAYER_MOVE_RIGHT, Input::JOYPAD_NO::PAD2)) {
//				curGravityDir_ = GRAVITY_DIR::RIGHT;
//			}
//		}
//
//		// 2. 共通の物理計算（自分の curGravityDir_ に基づいて加速）
//		VECTOR accel = AsoUtility::VECTOR_ZERO;
//		switch (curGravityDir_) {
//		case GRAVITY_DIR::UP:    accel.y = GRAVITY_ACCEL; break;
//		case GRAVITY_DIR::DOWN:  accel.y = -GRAVITY_ACCEL; break;
//		case GRAVITY_DIR::LEFT:  accel.x = -GRAVITY_ACCEL; break;
//		case GRAVITY_DIR::RIGHT: accel.x = GRAVITY_ACCEL; break;
//		default: break;
//		}
//
//		// 速度の更新
//		movePow_ = VAdd(movePow_, accel);
//
//		// 速度上限
//		if (VSize(movePow_) > TERMINAL_VELOCITY) {
//			movePow_ = VScale(VNorm(movePow_), TERMINAL_VELOCITY);
//		}
//
//		// 移動中アニメーション
//		if (curGravityDir_ != GRAVITY_DIR::NONE) {
//			PlayAnim(ANIM_TYPE::RUN);
//		}
//	}
//}
void Player::ProcessMove(void)
{
	//MOVEステージ（既存の挙動を完全に保護）
	if (stageType_ == static_cast<int>(STAGE_TYPE::MOVE)) 
	{
		VECTOR dir = AsoUtility::VECTOR_ZERO;

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

		if (!AsoUtility::EqualsVZero(dir))
		{
			moveSpeed_ = SPEED_MOVE;
			if (!isJump_) { PlayAnim(ANIM_TYPE::RUN); }

			Quaternion cameraRot = sceneMng_.GetCamera()->GetQuaRotY();
			moveDir_ = Quaternion::PosAxis(cameraRot, dir);
			movePow_ = VScale(moveDir_, moveSpeed_);
		}
		else
		{
			movePow_ = AsoUtility::VECTOR_ZERO;
			if (!isJump_) { PlayAnim(ANIM_TYPE::IDLE); }
		}

		return;
	}

	//GRAVITYステージ（新しい重力操作）
	if (stageType_ == static_cast<int>(STAGE_TYPE::GRAVITY)) 
	{
		// 入力による重力方向の切り替え（他方のプレイヤーには影響しない）
		if (playerNo_ == PLAYER_NO::P1) {
			if (input_.IsTrgDown(InputManager::TYPE::GRAVITY_MOVE_UP, Input::JOYPAD_NO::PAD1)) { curGravityDir_ = GRAVITY_DIR::UP; }
			if (input_.IsTrgDown(InputManager::TYPE::GRAVITY_MOVE_DOWN, Input::JOYPAD_NO::PAD1)) { curGravityDir_ = GRAVITY_DIR::DOWN; }
		}
		else if (playerNo_ == PLAYER_NO::P2) {
			if (input_.IsTrgDown(InputManager::TYPE::GRAVITY_MOVE_LEFT, Input::JOYPAD_NO::PAD2)) { curGravityDir_ = GRAVITY_DIR::LEFT; }
			if (input_.IsTrgDown(InputManager::TYPE::GRAVITY_MOVE_RIGHT, Input::JOYPAD_NO::PAD2)) { curGravityDir_ = GRAVITY_DIR::RIGHT; }
		}

		// 物理計算：現在の方向に基づいて加速
		VECTOR accel = AsoUtility::VECTOR_ZERO;
		switch (curGravityDir_) {
		case GRAVITY_DIR::UP:    accel.y = GRAVITY_ACCEL; break;
		case GRAVITY_DIR::DOWN:  accel.y = -GRAVITY_ACCEL; break;
		case GRAVITY_DIR::LEFT:  accel.x = -GRAVITY_ACCEL; break;
		case GRAVITY_DIR::RIGHT: accel.x = GRAVITY_ACCEL; break;
		default: break;
		}

		// movePow_ に加速度を足し続ける（速度が蓄積される）
		movePow_ = VAdd(movePow_, accel);

		// 最高速度制限
		if (VSize(movePow_) > TERMINAL_VELOCITY) {
			movePow_ = VScale(VNorm(movePow_), TERMINAL_VELOCITY);
		}

		// アニメーション制御
		if (curGravityDir_ != GRAVITY_DIR::NONE) {
			PlayAnim(ANIM_TYPE::RUN);
		}
		else {
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

