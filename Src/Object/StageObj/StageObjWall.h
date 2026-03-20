#pragma once
#include "./StageObjBase.h"
#include <DxLib.h>

class StageObjWall : public StageObjBase
{
public:

	StageObjWall(int _objType, float _alpha = 1.0f, bool isCollision = true);

	/// @brief 固定オブジェクトのため、更新しない
	void Update(void)override {};


protected:

	static constexpr float BLOCK_SCALE = 1.0f;
	static constexpr float COLLISION_SIZE_X = 200.0f;
	static constexpr float COLLISION_SIZE_Y = 200.0f;


	void InitLoad(void)override;

	void InitTransform(void)override;

	void InitCollider(void)override;


private:

	bool isCollision_;
};