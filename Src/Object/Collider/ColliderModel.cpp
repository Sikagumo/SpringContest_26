#include "ColliderModel.h"
#include <string>
#include <vector>
#include "./ColliderBase.h"
#include "../Common/Transform.h"


ColliderModel::ColliderModel(TAG _tag, const Transform* _follow)
	: ColliderBase(SHAPE::MODEL, _tag, _follow)
	, excludeFrameIds_{}, targetFrameIds_{}
{
}

void ColliderModel::AddExcludeFrameIds(const std::string& _name)
{
	// フレーム数を取得
	int num = MV1GetFrameNum(follow_->modelId);
	for (int i = 0; i < num; i++)
	{
		// フレーム名称を取得
		std::string frameName = MV1GetFrameName(follow_->modelId, i);

		if (frameName.find(_name) != std::string::npos)
		{
			// 除外フレームに追加(ホワイトリスト)
			excludeFrameIds_.emplace_back(i);
		}
	}
}

void ColliderModel::ClearExcludeFrame(void)
{
	excludeFrameIds_.clear();
}

bool ColliderModel::IsExcludeFrame(int _frameIdx) const
{
	// 除外判定
	if (std::find(excludeFrameIds_.begin(),
				  excludeFrameIds_.end(),
				  _frameIdx) != excludeFrameIds_.end())
	{
		// 除外に該当する
		return true;
	}
	return false;
}

void ColliderModel::AddTargetFrameIds(const std::string& _name)
{
	// フレーム数を取得
	int num = MV1GetFrameNum(follow_->modelId);
	for (int i = 1; i < num; i++)
	{
		// フレーム名称を除外
		std::string frameName = MV1GetFrameName(follow_->modelId, i);
		if (frameName.find(_name) != std::string::npos)
		{
			continue;
		}
		// 対象フレームに追加(ブラックリスト)
		targetFrameIds_.emplace_back(i);
	}
}

void ColliderModel::ClearTargetFrame(void)
{
	targetFrameIds_.clear();
}

bool ColliderModel::IsTargetFrame(int _frameIdx) const
{
	// 割り当て判定
	if (std::find(targetFrameIds_.begin(),
				  targetFrameIds_.end(), _frameIdx) == targetFrameIds_.end())
	{
		// 対象に該当する
		return true;
	}
	return false;
}

MV1_COLL_RESULT_POLY ColliderModel::GetNearestHitPolyLine(const VECTOR& _start, const VECTOR& _end
														  , bool _isExclude, bool _isTarget) const
{
	// 線分で衝突判定
	auto hits = MV1CollCheck_LineDim(
		follow_->modelId,
		-1,
		_start,
		_end
	);

	// 追従対象に一番近い衝突点を探す
	bool isCollision = false;
	MV1_COLL_RESULT_POLY hitPoly = MV1_COLL_RESULT_POLY();
	double minDist = DBL_MAX;

	for (int i = 0; i < hits.HitNum; i++)
	{
		const auto& hit = hits.Dim[i];

		if (_isExclude)
		{
			// 除外フレームは無視する
			if (IsExcludeFrame(hit.FrameIndex)) { continue; }

			// 対象フレームは無視する
			if (!IsTargetFrame(hit.FrameIndex)) { continue; }
		}


		// 衝突判定
		isCollision = true;

		// 距離判定
		float dist = VSize(VSub(hit.HitPosition, _start));

		if (minDist > dist/* && minDist > VIEW_NEAR*/)
		{
			// 追従対象に一番近い衝突点を優先
			minDist = dist;
			hitPoly = hit;
		}
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);
	
	return hitPoly;
}