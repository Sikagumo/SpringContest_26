#include "StageObjGoal.h"
#include "./StageObjBase.h"
#include "../Collider/ColliderPlane.h"
#include "../Collider/ColliderModel.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"

StageObjGoal::StageObjGoal(int _objType) :
	StageObjBase::StageObjBase({ COLLISION_SIZE_X, COLLISION_SIZE_Y }, _objType)
{
}

void StageObjGoal::InitLoad(void)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_GOAL));

}

void StageObjGoal::InitTransform(void)
{
	transform_.InitTransform(BLOCK_SCALE,
		Quaternion::Identity(), Quaternion::AngleAxis(-90.0f, AsoUtility::AXIS_X));
}

void StageObjGoal::InitCollider(void)
{
	ColliderModel* model = new ColliderModel(ColliderBase::TAG::GOAL, &transform_);
	ownColliders_.emplace(static_cast<int>(ColliderBase::SHAPE::MODEL), model);
}
