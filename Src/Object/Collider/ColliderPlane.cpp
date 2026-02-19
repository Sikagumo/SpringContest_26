#include "ColliderPlane.h"
#include "./ColliderBase.h"
#include "../Common/Transform.h"
#include "../../Utility/AsoUtility.h"
#include <DxLib.h>
#include <array>


ColliderPlane::ColliderPlane(TAG _tag, const Transform* _follow,
							 const VECTOR& _pos, Vector2F _size, bool _isMove)
	: ColliderBase::ColliderBase(SHAPE::PLANE, _tag, _follow),
	size_(_size), pos_(_pos),
	isMove_(_isMove)
{
	dirCollPos_.clear();
}

ColliderPlane::CollisionSide ColliderPlane::GetCollisionSide(int _dir)const
{
	/* 指定の方向の矩形の当たり判定を取得 */

	// 位置補正値
	const float OFFSET = 1.0f;

	CollisionSide collisionSide = CollisionSide::CollisionSide();

	// マイナス方向(左と下)の時、マイナス
	const int DIR_MINUS = ((_dir % 2 == 0) ? -1 : 1);

	// 上下の当たり判定
	if (_dir < (static_cast<int>(DIR::MAX) / 2))
	{
		// 中央
		collisionSide.mid = Vector2F(pos_.x, pos_.y);
		collisionSide.mid.y += ((size_.y + OFFSET) * DIR_MINUS);

		// 左
		collisionSide.start = collisionSide.mid;
		collisionSide.start.x -= (size_.x + OFFSET);

		// 右
		collisionSide.end = collisionSide.mid;
		collisionSide.end.x += (size_.x + OFFSET);
	}

	// 左右の当たり判定
	else
	{
		// 下
		collisionSide.end = Vector2F(pos_.x, pos_.y);
		collisionSide.end.x += ((size_.x + OFFSET) * DIR_MINUS);

		// 中央
		collisionSide.mid = collisionSide.end;
		collisionSide.mid.y += (size_.y + OFFSET);

		// 上
		collisionSide.start = collisionSide.mid;
		collisionSide.start.y += (size_.y + OFFSET);
	}

	collisionSide.dir = _dir;

	return collisionSide;
}

std::array<ColliderPlane::CollisionSide, static_cast<int>(ColliderPlane::DIR::MAX)>
	ColliderPlane::GetCollisionSides(void)const
{
	const int MAX = static_cast<int>(ColliderPlane::DIR::MAX);
	std::array<CollisionSide, MAX> array;

	CollisionSide collision;

	// 位置補正値
	const float OFFSET = 1.0f;


	// 当たり判定位置更新
	for (int i = 0; i < MAX; i++)
	{
		// マイナス方向(左と下)の時、マイナス
		const int DIR_MINUS = ((i % 2 == 0) ? -1 : 1);

		// 上下の当たり判定
		if (i < (static_cast<int>(DIR::MAX) / 2))
		{
			// 中央
			collision.mid = Vector2F(pos_.x, pos_.y);
			collision.mid.y += ((size_.y + OFFSET) * DIR_MINUS);

			// 左
			collision.start = collision.mid;
			collision.start.x -= (size_.x + OFFSET);

			// 右
			collision.end = collision.mid;
			collision.end.x += (size_.x + OFFSET);
		}

		// 左右の当たり判定
		else
		{
			// 下
			collision.end = Vector2F(pos_.x, pos_.y);
			collision.end.x += ((size_.x + OFFSET) * DIR_MINUS);

			// 中央
			collision.mid = collision.end;
			collision.mid.y += (size_.y + OFFSET);

			// 上
			collision.start = collision.mid;
			collision.start.y += (size_.y + OFFSET);
		}

		array[i] = collision;
	}
	
	return array;
}

void ColliderPlane::DrawDebug(int color)
{

}

VECTOR ColliderPlane::GetPosPushBackAlongNormal(const ColliderPlane& planeCollider2,
	int maxTryCnt, float pushDistance) const
{
	// コピー生成
	Transform tempTransform = *follow_;
	ColliderPlane tempPlane = *this;
	tempPlane.SetFollow(&tempTransform);

	// 衝突補正処理
	int tryCnt = 0;

	bool isLoop = true;

	while (isLoop && tryCnt++ < maxTryCnt)
	{
		isLoop = false;

		// 当たり判定が離れたら反発終了
		for (int i = 0; i < 4; i++)
		{
			// 対象と衝突していないとき、スキップ
			if (!IsHitCheck_Plane(planeCollider2.GetCollisionSide(i))) { continue; }

			// 再度反発処理を行う
			isLoop = true;

			// 反発を行う
			tempTransform.pos = _PushBack(tempTransform.pos, i, 10.0f);
		}
	}
	return tempTransform.pos;
}
VECTOR ColliderPlane::_PushBack(const VECTOR& _target, int _dirType, float pushDistance)const
{
	/* 反発処理 */
	VECTOR ret = _target;
	VECTOR pushDir = AsoUtility::VECTOR_ZERO;
	const int MAX = static_cast<int>(DIR::MAX);

	// マイナス方向(左と下)の時、処理を反転
	const float DIR_PUSH = ((_dirType % 2 == 0) ? -1.0f : 1.0f);

	// 方向の種類が上下の時、Y方向を向かせる
	pushDir = ((_dirType < (MAX / 2)) ?
						VGet(0.0f, DIR_PUSH, 0.0f) :
						VGet(DIR_PUSH, 0.0f, 0.0f));

	// 全ての指定方法に反発
	ret = VAdd(ret, VScale(pushDir, pushDistance));

	return ret;
}


bool ColliderPlane::IsHitCheck_Plane(CollisionSide _collider2)const
{
	/* 衝突判定 */

	bool ret = false;
	bool isStart, isMiddle, isEnd;

	// 対象の方向別の当たり判定
	CollisionSide targetColl = _collider2;

	// 自身の方向別の当たり判定
	CollisionSide thisColl = GetCollisionSide(targetColl.dir);

	// マイナス方向(左と下)の時、処理を反転
	const int DIR_MINUS = ((thisColl.dir % 2 == 0) ? -1 : 1);		


	// 上下の当たり判定
	if (targetColl.dir < (static_cast<int>(DIR::MAX) / 2))
	{
		isMiddle = ((thisColl.mid.y * DIR_MINUS) > (targetColl.mid.y * DIR_MINUS));

		isStart = (thisColl.start.x > targetColl.start.x);

		isEnd = (thisColl.end.x < targetColl.end.x);
	}

	// 左右の当たり判定
	else
	{
		isMiddle = ((thisColl.mid.x * DIR_MINUS) > (targetColl.mid.x * DIR_MINUS));

		isStart = (thisColl.start.y > targetColl.start.y);

		isEnd = (thisColl.end.y < targetColl.end.y);
	}

	// 対象のいずれかの当たり判定と衝突時、trueにして判定終了
	if (isMiddle || isStart || isEnd)
	{
		ret = true;
	}

	return ret;
}