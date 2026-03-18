#pragma once
#include<DxLib.h>
#include "SceneBase.h"
class SkyDome;
class StageBase;
class Player;

class GameScene : public SceneBase
{

public:
	
	enum class STAGE_TYPE
	{
		MOVE,
		GRAVITY,
		MAX,
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

	STAGE_TYPE GetStageType(void){ return stageType_; };


private:

	SkyDome* skyDome_;

	STAGE_TYPE stageType_;
	StageBase* stage_;

	Player* player1_;

	Player* player2_;
	void SetStageType(STAGE_TYPE _type);

	// 入れ替え権限の定義
	enum class SWAP_RIGHT {
		P1,
		P2
	};
	SWAP_RIGHT currentSwapRight_ = SWAP_RIGHT::P1;

	//イージング用に追加
	//入れ替え中かどうかのフラグ
	bool isSwapping_ = false;
	//現在の経過フレーム数
	float swapTimer_ = 0.0f;
	//入れ替えにかける合計フレーム数
	const float SWAP_LIMIT_FRAME = 30.0f;

	//移動の開始座標と終了座標を保持
	VECTOR p1StartPos_;
	VECTOR p1EndPos_;
	VECTOR p2StartPos_;
	VECTOR p2EndPos_;
};
