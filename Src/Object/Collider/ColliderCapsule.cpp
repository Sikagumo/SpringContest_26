#include "ColliderCapsule.h"
#include <DxLib.h>
#include "../Common/Transform.h"
#include "./ColliderModel.h"


ColliderCapsule::ColliderCapsule(TAG _tag, const Transform* _follow, const VECTOR& _localPosTop, const VECTOR& _localPosDown, float _radius):
	ColliderBase(SHAPE::CAPSULE, _tag, _follow),
	localPosTop_(_localPosTop),
	localPosDown_(_localPosDown),
	radius_(_radius)
{
}

VECTOR ColliderCapsule::GetPosTop(void) const
{
	return GetRotPos(localPosTop_);
}
VECTOR ColliderCapsule::GetPosDown(void) const
{
	return GetRotPos(localPosDown_);
}

VECTOR ColliderCapsule::GetCenter(void) const
{
	/* 正面座標を取得 */
	VECTOR top = GetPosTop();
	VECTOR down = GetPosDown();
	VECTOR diff = VSub(top, down);

	return VAdd(down, VScale(diff, 0.5f));
}

void ColliderCapsule::DrawDebug(unsigned int _color)
{
	/* デバッグ描画 */

	// 上の球体
	VECTOR pos1 = GetPosTop();
	DrawSphere3D(pos1, radius_, 5, _color, _color, false);

	// 下の球体
	VECTOR pos2 = GetPosDown();
	DrawSphere3D(pos2, radius_, 5, _color, _color, false);
	
	VECTOR dir;
	VECTOR s, e;

	// 球体を繋ぐ線(X+)
	dir = follow_->GetRight();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, _color);

	// 球体を繋ぐ線(X-)
	dir = follow_->GetLeft();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, _color);

	// 球体を繋ぐ線(Z+)
	dir = follow_->GetForward();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, _color);

	// 球体を繋ぐ線(Z-)
	dir = follow_->GetBack();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, _color);

	// カプセルの中心
	DrawSphere3D(GetCenter(), radius_, 10, _color, _color, true);
}

VECTOR ColliderCapsule::GetPosPushBackAlongNormal(const MV1_COLL_RESULT_POLY& _hitColPoly,
												  int _maxTryCnt, float _pushDistance) const
{
	/* 当たり判定で押し戻した座標を取得 */

	// コピー生成
	Transform tempTransform = *follow_;
	ColliderCapsule tmpCapsule = *this;
	tmpCapsule.SetFollow(&tempTransform);

	// 衝突補正処理
	int tryCnt = 0;
	while (tryCnt < _maxTryCnt)
	{
		// カプセルと三角形の当たり判定
		if (!HitCheck_Capsule_Triangle(
			tmpCapsule.GetPosTop(), tmpCapsule.GetPosDown(),
			tmpCapsule.GetRadius(),
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

void ColliderCapsule::PushBackAlongNormal(const ColliderModel* _colliderModel, Transform* _transform, int _maxTryCnt, float _pushDistance, bool _isExclude, bool _isTarget) const
{
	/* 当たり判定の押し戻し処理 */
	auto hits = MV1CollCheck_Capsule(_colliderModel->GetFollow()->modelId, -1,
									 GetPosTop(), GetPosDown(), GetRadius());

	for (int i = 0; i < hits.HitNum; i++)
	{
		auto hitPoly = hits.Dim[i];

		// 除外フレームは無視
		/*if (isExclude && colliderModel->IsExcludeFrame(hitPoly.FrameIndex))
		{
			continue;
		}*/
		// 対象フレーム以外は無視
		if (_isTarget && !_colliderModel->IsTargetFrame(hitPoly.FrameIndex))
		{
			continue;
		}

		// 指定回数と距離で三角形の法線方向に押し戻す
		_transform->pos = GetPosPushBackAlongNormal(hitPoly, _maxTryCnt, _pushDistance);
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);
}

bool ColliderCapsule::IsHit(const ColliderModel* colliderModel, bool _isExcude, bool _isTarget) const
{
	/* 衝突判定 */
	bool ret = false;

	// ステージモデル(地面)との衝突
	auto hits = MV1CollCheck_LineDim(
		colliderModel->GetFollow()->modelId, -1,
		GetPosTop(), GetPosDown());

	for (int i = 0; i < hits.HitNum; i++)
	{
		auto hit = hits.Dim[i];

		// 除外フレームは無視
		/*if (isExclude && colliderModel->IsExcludeFrame(hit.FrameIndex))
		{
			continue;
		}*/
		// 対象フレーム以外は無視
		if (_isTarget && !colliderModel->IsTargetFrame(hit.FrameIndex))
		{
			continue;
		}

		// 衝突！
		ret = true;
		break;
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);

	return ret;
}
