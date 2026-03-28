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

	//入れ替え演出
	void UpdateSwap(void);

	//罠復活演出
	void UpdateRespawn(void);

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

	//復活演出用に追加
	bool  isRespawning_ = false;       // 復活演出中かどうかのフラグ
	float respawnTimer_ = 0.0f;        // 復活演出の経過フレーム
	const float RESPAWN_LIMIT_FRAME = 60.0f; // 1秒（60フレーム）かけて戻る

	// 復活地点
	VECTOR p1InitialPos_;
	VECTOR p2InitialPos_;

	// 死亡した瞬間の座標（イージングの開始地点）
	VECTOR p1DeathPos_;
	VECTOR p2DeathPos_;
};
