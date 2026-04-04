#pragma once
#include "./StageBase.h"
#include <vector>
#include <DxLib.h>
class ActorBase;

class StageController
{
public:

	enum class STAGE_TYPE
	{
		NONE = -1,
		MOVE,
		MOVE3D,
		GRAVITY,
		GRAVITY3D,
		GAME_CLEAR
	};

	StageController(bool _isMoveStage);
	~StageController(void) = default;

	void Init(void);

	void Update(void);

	void Draw(void);

	void DrawPre(void);

	void DrawDebug(void);

	void Release(void);

	/// @brief 指定した状態と現在の状態が一致しているか否か
	/// @param _type ステージ状態
	bool GetIsTypeEquals(STAGE_TYPE _type) { return (stageType_ == _type);  };

	STAGE_TYPE GetStageType(void) { return stageType_; };

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

	const std::vector<STAGE_TYPE> STAGE_MOVE_TYPE
				= { STAGE_TYPE::MOVE3D, STAGE_TYPE::MOVE, STAGE_TYPE::MOVE3D, STAGE_TYPE::MOVE3D, STAGE_TYPE::MOVE3D };

	const std::vector<STAGE_TYPE> STAGE_GRAVITY_TYPE
				= { STAGE_TYPE::GRAVITY, STAGE_TYPE::GRAVITY, STAGE_TYPE::GRAVITY3D,  STAGE_TYPE::GRAVITY3D, STAGE_TYPE::GRAVITY3D };

	// ステージ数
	const int STAGE_SIZE_MAX;

	// 移動ステージか否か
	bool isMoveStage_;

	int curStageNum_;

	STAGE_TYPE stageType_;

	StageBase* stage_;

	bool isStageClear_;

	// クリアしたステージ番号
	std::vector<int> clearStageNums_;


	void SetStageType(STAGE_TYPE _type);

	/// @brief クリアしたステージ番号以外を抽選
	/// @return ステージ番号
	int GetStageChoice(void);
};