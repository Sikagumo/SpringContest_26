#pragma once
#include "./StageBase.h"
#include <vector>
#include <DxLib.h>
class ActorBase;

class StageController
{
public:

	// ステージの種類
	enum class STAGE_TYPE
	{
		NONE = -1,
		MOVE,		// 2D移動ステージ
		MOVE3D,		// 3D移動ステージ
		GRAVITY,	// 2D重力ステージ
		GRAVITY3D,	// 3D重力ステージ
		GAME_CLEAR,	// ゲームクリア
	};

	/// @brief コンストラクタ
	/// @param _isMoveStage 移動ステージか否か
	StageController(bool _isMoveStage);

	/// @brief デフォルトデストラクタ
	~StageController(void) = default;

	/// @brief 初期化処理
	void Init(void);

	/// @brief 更新処理
	void Update(void);

	/// @brief 描画処理
	void Draw(void);

	/// @brief 描画前処理
	void DrawPre(void);

	/// @brief デバッグ描画処理
	void DrawDebug(void);

	/// @brief メモリ開放処理
	void Release(void);


	/// @brief 指定した状態と現在の状態が一致しているか否か
	/// @param _type ステージ状態
	bool GetIsTypeEquals(STAGE_TYPE _type) { return (curStageType_ == _type);  };

	STAGE_TYPE GetStageType(void) { return curStageType_; };

	const VECTOR& GetPlayerPos(int _num) { return stage_->GetPlayerPos(_num); };

	const VECTOR& GetGoalPos(int _num);

	const std::vector<VECTOR>& GetTrapPos(void) const { return stage_->GetTrapPos(); }


	/// @brief ステージの当たり判定を全登録
	/// @param _actor 割り当てる対象
	void AddStageColliders(ActorBase& _actor) { stage_->AddStageColliders(_actor); };

	void ChangeStages(void);

	void SetIsStageClear(void) { isStageClear_ = true; };
	bool GetIsStageClear(void) { return isStageClear_; };


private:

	// 移動ステージの進行順
	const std::vector<STAGE_TYPE> STAGE_MOVE_TYPE
				= { STAGE_TYPE::MOVE, STAGE_TYPE::MOVE, STAGE_TYPE::MOVE3D, STAGE_TYPE::MOVE3D, STAGE_TYPE::MOVE3D };

	// 重力ステージの進行順
	const std::vector<STAGE_TYPE> STAGE_GRAVITY_TYPE
				= { STAGE_TYPE::GRAVITY, STAGE_TYPE::GRAVITY, STAGE_TYPE::GRAVITY3D,  STAGE_TYPE::GRAVITY3D, STAGE_TYPE::GRAVITY3D };

	// ステージ数
	const int STAGE_SIZE_MAX;

	// 移動ステージか否か
	bool isMoveStage_;

	// 現在のステージ進行数
	int curStageNum_;

	STAGE_TYPE curStageType_;

	StageBase* stage_;

	bool isStageClear_;

	// クリアしたステージ番号
	std::vector<int> clearStageNums_;


	void SetStageType(STAGE_TYPE _type);

	/// @brief クリアしたステージ番号以外を抽選
	/// @return ステージ番号
	int GetStageChoice(void);
};