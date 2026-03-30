#include "StageObjTrap.h"
#include "./StageObjBase.h"
#include "../Collider/ColliderSphere.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"

StageObjTrap::StageObjTrap(int _x, int _y, int _objType, float _alpha) :
	StageObjBase::StageObjBase(_x, _y, { COLLISION_SIZE_X, COLLISION_SIZE_Y }, _objType, _alpha)
{
}

void StageObjTrap::InitLoad(void)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_TRAP));

}

void StageObjTrap::InitTransform(void)
{
	transform_.InitTransform(BLOCK_SCALE,
		Quaternion::Identity(), Quaternion::AngleAxis(-90.0f, AsoUtility::AXIS_X));
}

void StageObjTrap::InitCollider(void)
{
	ColliderSphere* sphere = new ColliderSphere(ColliderBase::TAG::GOAL, &transform_, MODEL_OFFSET, COLLISION_RADIUS);
	ownColliders_.emplace(static_cast<int>(ColliderBase::SHAPE::SPHERE), sphere);
}
