#pragma once
#include<DxLib.h>
#include "SceneBase.h"
class SkyDome;
class StageBase;
class StageController;
class Player;

class GameScene : public SceneBase
{

public:

	enum class GAME_STATE
	{
		NONE = -1,
		ACTIVE,
		PAUSE,
		CLEAR,
	};
	
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


private:

	//入れ替えにかける合計フレーム数
	static constexpr float SWAP_LIMIT_FRAME = 30.0f;
	
	static constexpr float GOAL_HIT_RANGE = 80.0f;
	
	GAME_STATE state_;

	SkyDome* skyDome_;

	StageController* stage_;

	Player* player1_;

	Player* player2_;

	// 交代したか否か
	bool isExecuteSwaped_;


	// 入れ替え権限の定義
	enum class SWAP_RIGHT
	{
		P1,
		P2
	};
	SWAP_RIGHT currentSwapRight_ = SWAP_RIGHT::P1;

	// プレイヤーが入れ替え中かどうかのフラグ
	bool isSwapping_ = false;
	
	//現在の経過フレーム数
	float swapTimer_ = 0.0f;

	//移動の開始座標と終了座標を保持
	VECTOR p1StartPos_;
	VECTOR p1EndPos_;
	VECTOR p2StartPos_;
	VECTOR p2EndPos_;
	

	// プレイヤー入れ替え処理
	void PlayerSwap(void);

	void SetStageType(void);
};
