#pragma once
#include "./StageObjBase.h"
#include "../../Common/Vector2.h"
#include <DxLib.h>

class StageObjWall : public StageObjBase
{
public:

	/// @brief コンストラクタ
	/// @param _arrayPos ステージの配列位置
	/// @param _objType オブジェクトの種類
	/// @param _alpha 透過率(0.0～1.0)
	/// @param _isCollision 衝突させるか否か
	StageObjWall(Vector2 _arrayPos, int _objType, float _alpha = 1.0f, bool _isCollision = true);

	~StageObjWall(void)override = default;

	/// @brief 固定オブジェクトのため、更新しない
	void Update(void)override {};


protected:

	static constexpr float BLOCK_SCALE = 1.0f;
	static constexpr float COLLISION_SIZE = 200.0f;


	void InitLoad(void)override;

	void InitTransform(void)override;

	void InitCollider(void)override;


private:

	// 衝突させるか否か
	bool isCollision_;
};