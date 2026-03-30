#pragma once
#include<DxLib.h>
#include "SceneBase.h"
#include "../Utility/AsoUtility.h"
class SkyDome;
class StageBase;
class StageController;
class Player;

class GameScene : public SceneBase
{

public:

	
	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) override = default;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

	// ステージ取得
	const StageController& GetStage(void)const { return *stage_; };

	//入れ替え演出
	void UpdateSwap(void);

	//罠復活演出
	void UpdateRespawn(void);
	

private:

	//入れ替えにかける合計フレーム数
	static constexpr float SWAP_LIMIT_FRAME = 30.0f;
	
	static constexpr float GOAL_HIT_RANGE = 80.0f;
	
	SkyDome* skyDome_;

	StageController* stage_;

	struct PlayerParam
	{
		PlayerParam(void):
			player(nullptr),
			startPos(AsoUtility::VECTOR_ZERO),endPos(AsoUtility::VECTOR_ZERO),
			initialPos(AsoUtility::VECTOR_ZERO), deathPos(AsoUtility::VECTOR_ZERO)
		{}
		Player* player;

		//移動の開始座標と終了座標を保持
		VECTOR startPos;
		VECTOR endPos;

		// 復活地点
		VECTOR initialPos;

		// 死亡した瞬間の座標（イージングの開始地点）
		VECTOR deathPos;
	};

	PlayerParam player1_;
	PlayerParam player2_;

	// 交代したか否か
	bool isExecuteSwaped_;


	// 入れ替え権限の定義
	enum class SWAP_RIGHT
	{
		P1,
		P2
	};
	SWAP_RIGHT currentSwapRight_ = SWAP_RIGHT::P1;

	//入れ替え中かどうかのフラグ
	bool isSwapping_ = false;

	//現在の経過フレーム数
	float swapTimer_ = 0.0f;

	/* 復活演出 */
	
	// 復活演出中かどうか
	bool isRespawning_ = false;
	
	// 復活演出の経過フレーム
	float respawnTimer_ = 0.0f;
	
	// 1秒（60フレーム）かけて戻る
	const float RESPAWN_LIMIT_FRAME = 60.0f;

	void SetStageType(void);

	/// @brief 罠の衝突処理
	/// @return 衝突時:true
	bool TrapProcess(void);

	/// @brief ゴール処理
	/// @return ゴールしているか否かの判定
	bool GoalProcess(void);
};
