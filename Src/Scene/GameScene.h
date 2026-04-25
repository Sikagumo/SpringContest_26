#pragma once
#include<DxLib.h>
#include<array>
#include <functional>
#include "SceneBase.h"
#include "../Utility/AsoUtility.h"
class SkyDome;
class StageBase;
class StageController;
class Player;
class GameTimer;

class GameScene : public SceneBase
{

public:

	enum class GAME_STATE
	{
		NONE = -1,
		INFO,
		INFO3D,
		ACTIVE,
		GOAL,
		PAUSE,
		GAME_CLEAR,
		GAME_OVER,
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

	//入れ替え演出
	void UpdateSwap(void);

	//罠復活演出
	void UpdateRespawn(void);
	

private:

	//入れ替えにかける合計フレーム数
	static constexpr float SWAP_LIMIT_FRAME = 0.5f;

	// 1秒（60フレーム）かけて戻る
	static constexpr float RESPAWN_LIMIT_FRAME = 1.0f;

	
	static constexpr float GOAL_HIT_RANGE = 80.0f;
	
	static constexpr float GAME_TIME = 300.0f;

	static constexpr int INFO_MAX = 3;
	static constexpr int INFO3D_MAX = 2;

	static constexpr float HIT_STOP_TRAP = 1.5f;

	static constexpr float TIME_START = 2.0f;
	static constexpr float TIME_PAUSE = 0.0f;
	static constexpr float TIME_GAME_END = 2.0f;
	static constexpr float TIME_CLEAR = 0.5f;
	
	static constexpr float UI_TEXT_SCALE = 0.5f;
	static constexpr int UI_TEXT_SIZE = static_cast<int>((80 * UI_TEXT_SCALE));

	int preStageType_;

	float performTime_;
	bool isPerform_;

	GAME_STATE state_;

	SkyDome* skyDome_;

	StageController* stage_;

	GameTimer* timer_;

	Player* player1_;
	Player* player2_;


	// プレイヤー１が交代するか否か
	bool isSwapPlayer1_;

	// ゲーム状態別の処理
	std::function<void(void)> updateGameStateProc_;

	//入れ替え中かどうかのフラグ
	bool isSwapping_ = false;

	//現在の経過フレーム数
	float swapTimer_ = 0.0f;

	// 現在の遊び方の番号
	int curInfoNum_;

	int infoImages_[((INFO_MAX + INFO3D_MAX) * 2)];


	/* 復活演出 */
	
	// 復活演出中かどうか
	bool isRespawning_ = false;
	
	// 復活演出の経過フレーム
	float respawnTimer_ = 0.0f;
	


	void SetStageType(void);

	/// @brief 罠の衝突処理
	/// @return 衝突時:true
	bool TrapProcess(void);

	/// @brief ゴール処理
	/// @return ゴールしているか否かの判定
	bool GoalProcess(void);

	void DrawTimer(void);
	void DrawInfo(void);

	void ChangeState(GAME_STATE _state);

	void Update_Active(void);
	void Update_Info(void);
	void Update_Goal(void);
	void Update_Pause(void);
	void Update_Clear(void);
	void Update_GameOver(void);
};
