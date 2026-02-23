#include "StageObjBase.h"
#include "../Actor/ActorBase.h"


StageObjBase::StageObjBase(const VECTOR& _collisionSize, int _objType):
	ActorBase::ActorBase(),
	collisionPos_(Vector2F(0.0f, 0.0f)),
	collisionSize_(Vector2F(_collisionSize.x, _collisionSize.y)),
	type(_objType)
{
}

void StageObjBase::Init(const VECTOR& _pos)
{
	ActorBase::Init();

	transform_.pos = _pos;

	collisionPos_.x = transform_.pos.x;
	collisionPos_.y = transform_.pos.y;
	transform_.Update();
}

const ColliderBase* StageObjBase::GetOwnCollider(void)
{
	if (ownColliders_.count(0) == 0)
	{
		return nullptr;
	}
	return ownColliders_.at(0);
}
