#include "StageObjWall.h"
#include <DxLib.h>
#include "./StageObjBase.h"
#include "../Collider/ColliderModel.h"
#include "../../Manager/ResourceManager.h"

StageObjWall::StageObjWall(Vector2 _arrayPos, int _objType, float _alpha, bool _isCollision)
	: StageObjBase::StageObjBase(_arrayPos, VGet(COLLISION_SIZE, COLLISION_SIZE, COLLISION_SIZE)
								 , _objType, _alpha)
	, isCollision_(_isCollision)
{
}

void StageObjWall::InitLoad(void)
{
	/* 初回読み込み処理 */
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE_STONE));

	transform_.SetAlpha(alpha_);
}

void StageObjWall::InitTransform(void)
{
	/* オブジェクトのトランスフォーム初期化 */
	transform_.InitTransform(BLOCK_SCALE
							 , Quaternion::Identity()
							 , Quaternion::Identity());
}

void StageObjWall::InitCollider(void)
{
	/* 当たり判定初期化処理　*/

	// 衝突させない場合、処理終了
	if (!isCollision_) { return; }


	ColliderModel* model = new ColliderModel(ColliderBase::TAG::STAGE, &transform_);
	ownColliders_.emplace(static_cast<int>(ColliderBase::SHAPE::MODEL), model);
}