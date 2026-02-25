#pragma once
#include "./StageObjBase.h"
#include <DxLib.h>

class StageObjGoal: public StageObjBase
{
public:

	StageObjGoal(int _objType);

	/// @brief 固定オブジェクトのため、更新しない
	void Update(void)override {};


protected:

	static constexpr float BLOCK_SCALE = 0.15f;
	static constexpr float COLLISION_RADIUS = 100.0f;
	static constexpr float COLLISION_SIZE_X = 10.0f;
	static constexpr float COLLISION_SIZE_Y = 10.0f;
	static constexpr VECTOR MODEL_OFFSET = { 0.0f, 0.0f, 0.0f };


	void InitLoad(void)override;

	void InitTransform(void)override;

	void InitCollider(void)override;
};