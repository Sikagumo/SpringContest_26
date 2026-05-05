#include "ColliderSphere.h"
#include "./ColliderBase.h"
#include "../Common/Transform.h"

ColliderSphere::ColliderSphere(TAG tag, const Transform* follow, const VECTOR& localPos, float radius)
	: ColliderBase::ColliderBase(SHAPE::SPHERE, tag, follow)
	, localPos_(localPos),radius_(radius)
{

}

VECTOR ColliderSphere::GetPos(void) const
{
	return GetRotPos(localPos_);
}

void ColliderSphere::DrawDebug(unsigned int _color)
{
	// ポリゴン分割数
	constexpr int DIV_NUM = 10;

	DrawSphere3D(GetPos(), radius_, DIV_NUM, _color, _color, true);
}

VECTOR ColliderSphere::GetPosPushBackAlongNormal(const MV1_COLL_RESULT_POLY& _hitColPoly
												 , int _maxTryCnt, float _pushDistance) const
{
	/* 反発後の座標を取得 */

	// コピー生成
	Transform tempTransform = *follow_;
	ColliderSphere tempSphere = *this;
	tempSphere.SetFollow(&tempTransform);

	// 衝突補正処理
	int tryCnt = 0;
	while (tryCnt < _maxTryCnt)
	{
		// カプセルと三角形の当たり判定
		if (!HitCheck_Sphere_Triangle(
			tempSphere.GetPos(),
			tempSphere.GetRadius(),
			_hitColPoly.Position[0], _hitColPoly.Position[1],
			_hitColPoly.Position[2]))
		{
			break;
		}

		// 衝突していたら法線方向に押し戻し
		tempTransform.pos = VAdd(tempTransform.pos, VScale(_hitColPoly.Normal, _pushDistance));
		tryCnt++;
	}
	return tempTransform.pos;
}