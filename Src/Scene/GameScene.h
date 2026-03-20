#pragma once
#include "SceneBase.h"
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


private:

	SkyDome* skyDome_;

	StageBase* stageB_;
	StageController* stage_;

	Player* player1_;

	Player* player2_;

	// 交代したか否か
	bool isExecuteSwaped_;


	// 入れ替え権限の定義
	enum class SWAP_RIGHT {
		P1,
		P2
	};
	SWAP_RIGHT curSwapRight_ = SWAP_RIGHT::P1;


	void PlayerSwap(void);

	void SetStageType(void);

};
