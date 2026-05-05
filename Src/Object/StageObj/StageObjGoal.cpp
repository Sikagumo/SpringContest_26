#include "StageObjGoal.h"
#include "./StageObjBase.h"
#include "../Collider/ColliderSphere.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/UtilityMath.h"

StageObjGoal::StageObjGoal(Vector2 _arrayPos, int _objType, float _alpha)
	: StageObjBase::StageObjBase(_arrayPos, VGet(COLLISION_SIZE, COLLISION_SIZE, COLLISION_SIZE)
								 , _objType, _alpha)
{
}

void StageObjGoal::InitLoad(void)
{
	/* 初回読み込み処理 */
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_GOAL));
}

void StageObjGoal::InitTransform(void)
{
	/* オブジェクトのトランスフォーム初期化 */
	const float LOCAL_ROT_X = -90.0f;
	transform_.InitTransform(BLOCK_SCALE
							 , Quaternion::Identity()
							 , Quaternion::AngleAxis(LOCAL_ROT_X, UtilityMath::AXIS_X));
}

void StageObjGoal::InitCollider(void)
{
	/* 当たり判定初期化処理　*/
	ColliderSphere* sphere = new ColliderSphere(ColliderBase::TAG::GOAL, &transform_, MODEL_OFFSET, COLLISION_RADIUS);
	ownColliders_.emplace(static_cast<int>(ColliderBase::SHAPE::SPHERE), sphere);
}
