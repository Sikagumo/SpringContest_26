#include "../Common/Transform.h"
#include "ColliderBase.h"


ColliderBase::ColliderBase(SHAPE shape, TAG tag, const Transform* follow)
	:
	shape_(shape),
	tag_(tag),
	follow_(follow),
	isValid_(true)
{
}


void ColliderBase::Draw(void)
{
	// デバッグ表示の色
	constexpr int COLOR_VALID = 0xff0000;
	constexpr int COLOR_INVALID = 0xaaaaaa;

	int color = COLOR_INVALID;
	if (isValid_)
	{
		color = COLOR_VALID;
	}
	DrawDebug(color);
}

VECTOR ColliderBase::GetRotPos(const VECTOR& _localPos) const
{
	// 追従相手の回転に合わせて指定ローカル座標を回転し、
	// 基準座標に加えることでワールド座標へ変換
	VECTOR localRotPos = follow_->quaRot.PosAxis(_localPos);
	return VAdd(follow_->pos, localRotPos);
}