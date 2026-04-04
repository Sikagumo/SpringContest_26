#include "StageObjBase.h"
#include "../Actor/ActorBase.h"
#include "../../Common/Vector2.h"


StageObjBase::StageObjBase(int _x, int _y, const VECTOR& _collisionSize, int _objType, float _alpha):
	ActorBase::ActorBase(),
	x(_x), y(_y),
	collisionPos_(Vector2F(0.0f, 0.0f)),
	collisionSize_(Vector2F(_collisionSize.x, _collisionSize.y)),
	objType_(_objType), alpha_(_alpha)
{
}

void StageObjBase::Init(const VECTOR& _pos)
{
	ActorBase::Init();

	transform_.pos = VAdd(transform_.pos, _pos);

	collisionPos_.x = transform_.pos.x;
	collisionPos_.y = transform_.pos.y;
	transform_.Update();
}

void StageObjBase::Draw(void)
{
	if (alpha_ < 1.0f)
	{
		// Zバッファへの書き込みを無効にして内側モデルを描画
		SetWriteZBuffer3D(FALSE);

		SetUseLighting(FALSE);

		// モデルの１番目のメッシュの描画ブレンドモード
		MV1SetMeshDrawBlendMode(transform_.modelId, 0, DX_BLENDMODE_ALPHA);
	}

	ActorBase::Draw();

	if (alpha_ < 1.0f)
	{
		SetUseLighting(TRUE);
		
		// Zバッファへの書き込みを無効にして内側モデルを描画
		SetWriteZBuffer3D(TRUE);

		// モデルの１番目のメッシュの描画ブレンドモード
		MV1SetMeshDrawBlendMode(transform_.modelId, 0, DX_BLENDMODE_NOBLEND);
	}
}

const ColliderBase* StageObjBase::GetOwnCollider(void)
{
	if (ownColliders_.count(0) == 0)
	{
		return nullptr;
	}
	return ownColliders_.at(0);
}

void StageObjBase::SetAlpha(float _alpha)
{
	alpha_ = _alpha;
	transform_.SetAlpha(_alpha);
}