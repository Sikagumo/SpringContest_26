#pragma once
#include <DxLib.h>
#include "../Actor/CharaBase.h"
#include "../../Manager/InputManager.h"

class Player : public CharaBase
{
public:

	enum class STATE
	{
		NONE = -1,
		ACTIVE,
		SWAP, // 移動,
		GOAL,
	};

	enum class PLAYER_NO
	{
		NONE = -1,
		P1,
		P2,
		MAX
	};

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 10.0f;


	Player(PLAYER_NO _playerNo, const VECTOR& _pos, STAGE_TYPE _stageType);

	~Player(void)override = default;

	void ReInit(const VECTOR& _pos, STAGE_TYPE _stageType);

	void Draw(void)override;


	void SetPlayerNo(PLAYER_NO no);
	PLAYER_NO GetPlayerNo(void) { return playerNo_; };

	void SetGameStageType(void);


	void SetIsChangeModel(bool _isChangeModel);
	bool GetIsChangeModel(void) { return isChangeModel_; };

	void ChangeState(STATE _state);
	STATE GetState(void) { return playerState_; };

	//交代権限を設定する
	void SetAuthority(bool hasAuth) { isHasAuthority_ = hasAuth; }
	
	// 重力方向に応じた回転を更新する
	void UpdateGravityRotation(void);

	/// @brief 開始位置を取得
	const VECTOR& GetInitialPos(void) { return initialPos_; };

	void SetEasingActive(const VECTOR& _endPos);

	void SetEasingPos(float _easingNum);

	void SetHitTrap(void);



protected:


	void InitLoadPost(void)override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void)override;

	// 衝突判定の初期化
	void InitCollider(void)override;

	// 初期化後の個別処理
	void InitPost(void)override;

	// 更新処理
	void UpdateProcess(void)override;

	// 
	void UpdateProcessPost(void)override;

	void CollisionReserve(void)override {};


private:
	
	// 落下加速度
	static constexpr float GRAVITY_ACCEL = 0.5f;   
	
	// 最高速度
	static constexpr float TERMINAL_VELOCITY = 10.0f; 

	static constexpr COLOR_F P1_COLOR = COLOR_F(1.0f, 0.25f, 0.25f, 1.0f);
	static constexpr COLOR_F P2_COLOR = COLOR_F(0.25f, 0.25f, 1.0f, 1.0f);


	// 衝突判定用線分のローカル位置
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 20.0f, 0.0f };
	static constexpr VECTOR COL_LINE_END_LOCAL_POS   = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_POS_P1 = { 0.0f, 0.0f, 30.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_POS_P1 = { 0.0f, 0.0f, -30.0f };

	// プレイヤー２衝突判定用カプセル球体位置
	static constexpr VECTOR COL_CAPSULE_TOP_POS_P2 = { 0.0f, 40.0f, 0.0f };
	static constexpr VECTOR COL_CAPSULE_DOWN_POS_P2 = { 0.0f, -40.0f, 0.0f };

	// 移動速度(通常)
	static constexpr float SPEED_MOVE = 7.5f;


	enum class GRAVITY_DIR { UP, DOWN, LEFT, RIGHT, NONE };
	GRAVITY_DIR curGravityDir_ = GRAVITY_DIR::NONE;

	// 入力
	InputManager& input_;

	// モデルの見た目を変更するか否か
	bool isChangeModel_;

	// モデルの目のフレーム番号
	int frameEyeDefault_;
	int frameEyeDamage_;

	// プレイヤー状態
	STATE playerState_;

	PLAYER_NO playerNo_;

	// 交代権限を持っているか否か
	bool isHasAuthority_;
	
	int arrowHandle_;

	// 開始地点
	VECTOR initialPos_;

	// イージング地点
	VECTOR easingPosStart_;
	VECTOR easingPosEnd_;

	// 操作
	void ProcessMove(void);
};