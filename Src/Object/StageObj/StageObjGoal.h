#pragma once
#include "./StageObjBase.h"
#include "../../Common/Vector2.h"
#include <DxLib.h>

class StageObjGoal: public StageObjBase
{
public:

	/// @brief コンストラクタ
	/// @param _arrayPos ステージの配列位置
	/// @param _objType オブジェクトの種類
	/// @param _alpha 透過率(0.0～1.0)
	/// @param _isCollision 衝突させるか否か
	StageObjGoal(Vector2 _arrayPos, int _objType, float _alpha = 1.0f);

	~StageObjGoal(void)override = default;


	/// @brief 固定オブジェクトのため、更新しない
	void Update(void)override {};


protected:

	static constexpr float BLOCK_SCALE = 0.15f;
	static constexpr float COLLISION_RADIUS = 100.0f;
	static constexpr float COLLISION_SIZE = 10.0f;
	static constexpr VECTOR MODEL_OFFSET = { 0.0f, 0.0f, 0.0f };


	void InitLoad(void)override;

	void InitTransform(void)override;

	void InitCollider(void)override;
};