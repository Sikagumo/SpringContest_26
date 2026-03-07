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

	const StageController& GetStage(void)const { return *stage_; };


private:

	SkyDome* skyDome_;

	StageBase* stageB_;
	StageController* stage_;

	Player* player1_;

	Player* player2_;
	void SetStageType(void);
};
