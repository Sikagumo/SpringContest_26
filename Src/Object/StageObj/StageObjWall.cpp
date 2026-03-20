#include "StageObjWall.h"
#include <DxLib.h>
#include "./StageObjBase.h"
#include "../Collider/ColliderModel.h"
#include "../Collider/ColliderPlane.h"
#include "../../Manager/ResourceManager.h"

StageObjWall::StageObjWall(int _x, int _y, int _objType, float _alpha, bool _isCollision) :
	StageObjBase::StageObjBase(_x, _y, { COLLISION_SIZE_X, COLLISION_SIZE_Y }, _objType, _alpha),
	isCollision_(_isCollision)
{
}

void StageObjWall::InitLoad(void)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE_STONE));

	transform_.SetAlpha(alpha_);
}

void StageObjWall::InitTransform(void)
{
	transform_.InitTransform(BLOCK_SCALE,
							 Quaternion::Identity(), Quaternion::Identity());
}

void StageObjWall::InitCollider(void)
{
	if (isCollision_)
	{
		// 3D•½–ÊƒRƒ‰ƒCƒ_“o˜^
		//ColliderPlane* plane = new ColliderPlane(ColliderBase::TAG::STAGE, &transform_,
												 //COLLISION_SIZE_X, COLLISION_SIZE_Y, false);

		//ownColliders_.emplace(static_cast<int>(ColliderBase::SHAPE::PLANE), plane);

		ColliderModel* model = new ColliderModel(ColliderBase::TAG::STAGE, &transform_);
		ownColliders_.emplace(static_cast<int>(ColliderBase::SHAPE::MODEL), model);
	}
}