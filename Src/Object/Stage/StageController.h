#pragma once
#include "./StageBase.h"
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
		CLEAR
	};

	StageController(void);
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

	const VECTOR& GetGoalPos(void) { return stage_->GetGoalPos(); };

	/// @brief ステージの当たり判定を全登録
	/// @param _actor 割り当てる対象
	void AddStageColliders(ActorBase& _actor) { stage_->AddStageColliders(_actor); };

	void ChangeStages(void);

	void SetIsStageClear(void) { isStageClear_ = true; };
	bool GetIsStageClear(void) { return isStageClear_; };


private:

	STAGE_TYPE stageType_;

	StageBase* stage_;

	bool isStageClear_;

	void SetStageType(STAGE_TYPE _type);
};