#pragma once
#include "../Actor/CharaBase.h"
#include "../../Manager/InputManager.h"

class Player : public CharaBase
{
public:

	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		MAX,
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


	Player(PLAYER_NO _playerNo, const VECTOR& _pos);

	~Player(void)override = default;

	void Init(const VECTOR& _pos, STAGE_TYPE _stageType);

	void Draw(void)override;

	void Release(void)override;

	void SetPlayerNo(PLAYER_NO no);
	PLAYER_NO GetPlayerNo(void) { return playerNo_; };

	void SetGameStageType(STAGE_TYPE stageType);


	void SetIsChangeModel(bool _isChangeModel);
	bool GetIsChangeModel(void) { return isChangeModel_; };

	void SetIsGoal(bool _isGoal) { isGoal_ = _isGoal; };
	bool GetIsGoal(void) { return isGoal_; };

	//交代権限を設定する
	void SetAuthority(bool hasAuth) { hasAuthority_ = hasAuth; }
	
	// 重力方向に応じた回転を更新する
	void UpdateGravityRotation(void);


protected:


	void InitLoadPost(void)override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void)override;

	// 衝突判定の初期化
	void InitCollider(void)override;

	// アニメーションの初期化
	void InitAnimationPost(void)override;

	// 初期化後の個別処理
	void InitPost(void)override;

	// 更新処理
	void UpdateProcess(void)override;

	// 
	void UpdateProcessPost(void)override;

	void CollisionReserve(void)override {};


private:

	enum class GRAVITY_DIR { UP, DOWN, LEFT, RIGHT, NONE };
	GRAVITY_DIR curGravityDir_ = GRAVITY_DIR::NONE;
	
	// 落下加速度
	const float GRAVITY_ACCEL = 0.5f;   
	
	// 最高速度
	const float TERMINAL_VELOCITY = 10.0f; 

	static constexpr COLOR_F P1_COLOR = COLOR_F(1.0f, 0.25f, 0.25f, 1.0f);
	static constexpr COLOR_F P2_COLOR = COLOR_F(0.25f, 0.25f, 1.0f, 1.0f);


	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 20.0f, 0.0f };

	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用線分開始(ジャンプ時)
	static constexpr VECTOR COL_LINE_JUMP_START_LOCAL_POS = { 0.0f, 30.0f, 0.0f };

	// 衝突判定用線分終了(ジャンプ時)
	static constexpr VECTOR COL_LINE_JUMP_END_LOCAL_POS = { 0.0f, 10.0f, 0.0f };


	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_POS_P1 = { 0.0f, 0.0f, 30.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_POS_P1 = { 0.0f, 0.0f, -30.0f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_POS_P2 = { 0.0f, 40.0f, 0.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_POS_P2 = { 0.0f, -40.0f, 0.0f };

	

	// ジャンプ力
	static constexpr float POW_JUMP_INIT = 5000.0f;

	// 持続ジャンプ力
	static constexpr float POW_JUMP_KEEP = 575.0f;

	// ジャンプ受付時間
	static constexpr float TIME_JUMP_INPUT = 0.6f;

	// 移動速度(通常)
	static constexpr float SPEED_MOVE = 7.5f;


	// 入力
	InputManager& input_;

	int lightHandle_;

	// モデルの見た目を変更するか否か
	bool isChangeModel_;

	// モデルの目のフレーム番号
	int frameEyeDefault_;
	int frameEyeDamage_;

	// ゴールしているか否か
	bool isGoal_;

	PLAYER_NO playerNo_;

	bool hasAuthority_;
	
	int arrowHandle_;


	// 操作
	void ProcessMove(void);

	// ジャンプ
	void ProcessJump(void);

	void PlayAnim(ANIM_TYPE objType_, bool _isLoop = true);
};