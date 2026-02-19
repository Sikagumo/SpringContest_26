#pragma once
#include <map>
#include <array>
#include <Dxlib.h>
#include "ColliderBase.h"
class Transform;

class ColliderPlane : public ColliderBase
{
public:

	enum class DIR
	{
		BOTTOM,
		TOP,
		LEFT,
		RIGHT,
		MAX
	};

	struct Vector2F
	{
		Vector2F(float _x, float _y) :
			x(_x), y(_y) { }

		Vector2F(void) :
			x(0.0f), y(0.0f) { }

		float x, y;
	};
	struct CollisionSide
	{
		CollisionSide(void) :
			dir(-1),
			start(Vector2F()), mid(Vector2F()), end(Vector2F())
		{ };

		int dir;
		Vector2F start;
		Vector2F mid;
		Vector2F end;
	};

	/// @brief コンストラクタ
	ColliderPlane(TAG _tag, const Transform* _follow,
				  const VECTOR& _pos, Vector2F _size, bool isMove);

	~ColliderPlane(void)override = default;

	void DrawDebug(int color)override;

	// 指定された回数と距離で衝突方向に押し戻した座標を取得
	VECTOR GetPosPushBackAlongNormal(const ColliderPlane& planeCollider2,
									int maxTryCnt,
									float pushDistance) const;

	/// @brief 各方向の当たり判定リスト取得
	const std::map<DIR, CollisionSide*>& GetSideCollisionPos(void)const { return dirCollPos_; };

	/// @brief 対象オブジェクトが反発時に移動するか否か
	bool IsObjectMove(void) { return isMove_; };

	/// @brief 指定の方向の矩形の当たり判定を取得
	CollisionSide GetCollisionSide(int _dir)const;

	std::array<CollisionSide, static_cast<int>(DIR::MAX)> GetCollisionSides(void)const;

private:

	// 各方向の当たり判定
	std::map<DIR, CollisionSide*> dirCollPos_;

	// 当たり判定サイズ
	Vector2F size_;

	// 対象の位置(下側から)
	VECTOR pos_;

	// 稼働するオブジェクトか否か
	bool isMove_;


	/// @brief 方向別に矩形との衝突判定を行う
	/// @param _collisionPos 衝突位置
	bool IsHitCheck_Plane(CollisionSide _collder2)const;

	/// @brief 反発処理
	/// @param _target 対象の元座標
	/// @param _dirType 方向の種類
	/// @param _pushDistance 反発力
	/// @return 反発後の座標
	VECTOR _PushBack(const VECTOR& _target, int _dirType, float _pushDistance)const;
};