#pragma once
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
};
