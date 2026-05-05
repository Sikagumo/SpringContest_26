#pragma once
#include <DxLib.h>
#include <memory>
#include <array>
#include <functional>
#include "SceneBase.h"
#include "../Utility/UtilityMath.h"
class SkyDome;
class StageBase;
class StageController;
class Player;
class GameTimer;

class GameScene : public SceneBase
{

public:

	/// @brief ゲーム状態
	enum class GAME_STATE
	{
		NONE = -1,

		INFO,	// 遊び方説明
		INFO3D, // 3Dステージの遊び方説明

		ACTIVE,		// プレイ中
		GOAL,		// ゴール
		PAUSE,		// 一時停止
		GAME_CLEAR, // ゲームクリア
		GAME_OVER,  // ゲームオーバー
	};
	
	GameScene(void);
	~GameScene(void) override = default;

	void Initialize(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;


	/// @brief ステージ取得
	const StageController& GetStage(void)const { return *stage_; };
	

private:

	//入れ替えにかける合計フレーム数
	static constexpr float SWAP_LIMIT_FRAME = 0.5f;

	// 1秒（60フレーム）かけて戻る
	static constexpr float RESPAWN_LIMIT_FRAME = 1.0f;

	static constexpr float GOAL_HIT_RANGE = 80.0f;

	static constexpr int INFO_MAX = 3;
	static constexpr int INFO3D_MAX = 2;


	// 前フレームのステージタイプ
	int preStageType_;

	// 演出時間
	float performTime_;
	bool isPerform_;

	// ゲーム状態
	GAME_STATE state_;

	// スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	// ステージ管理
	std::unique_ptr<StageController> stage_;

	// 
	std::unique_ptr<GameTimer> timer_;

	// 各プレイヤー
	Player* player1_;
	Player* player2_;

	// ゲーム状態別の処理
	std::function<void(void)> updateGameStateProc_;

	// プレイヤー１が交代するか否か
	bool isSwapPlayer1_;

	//入れ替え中かどうかのフラグ
	bool isSwapping_;

	// 現在の経過フレーム数
	float swapTimer_;

	// 現在の遊び方の番号
	int curInfoNum_;

	// 遊び方画像
	std::array<int, ((INFO_MAX + INFO3D_MAX) * 2)> infoImages_;


	/* 復活演出 */
	
	// 復活演出中かどうか
	bool isRespawning_;
	
	// 復活演出の経過フレーム
	float respawnTimer_;
	

	/// @brief 入れ替え演出
	void UpdateSwap(void);

	/// @brief 罠復活演出
	void UpdateRespawn(void);

	/// @brief 罠の衝突処理
	/// @return 衝突時:true
	bool TrapProcess(void);


	void SetStageType(void);


	/// @brief ゴール処理
	/// @return ゴールしているか否かの判定
	bool GoalProcess(void);

	/// @brief 遊び方描画処理
	void DrawInfo(void);

	/// @brief ゲーム状態遷移処理
	/// @param _state 遷移後状態
	void ChangeState(GAME_STATE _state);


	/* 状態別更新処理 */
	void Update_Active(void);
	void Update_Info(void);
	void Update_Goal(void);
	void Update_Pause(void);
	void Update_Clear(void);
	void Update_GameOver(void);
};
