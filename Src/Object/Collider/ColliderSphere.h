#pragma once
#include <DxLib.h>
#include "ColliderBase.h"
class Transform;


class ColliderSphere : public ColliderBase
{
public:

	/// @brief コンストラクタ
	/// @param _tag オブジェクトのタグ
	/// @param _follow 追従対象
	/// @param _localPos ローカル座標
	/// @param _radius 反発
	ColliderSphere(TAG _tag, const Transform* _follow, const VECTOR& _localPos, float _radius);

	~ColliderSphere(void)override = default;


	/// @brief 親Transformからの相対位置を取得
	const VECTOR& GetLocalPos(void) const { return localPos_; };

	/// @brief 親Transformからの相対位置を割り当て
	void SetLocalPos(const VECTOR& _localPos) { localPos_ = _localPos; };

	/// @brief ワールド座標を取得
	VECTOR GetPos(void) const;

	/// @brief 半径取得
	float GetRadius(void) const { return radius_; };

	/// @brief 半径割り当て
	void SetRadius(float _radius) { radius_ = _radius; };

	/// @brief 指定された回数と距離で三角形の法線方向に押し戻した座標を取得
	/// @param _hitColPoly ポリゴン
	/// @param _maxTryCnt 最大反発回数
	/// @param _pushDistance 反発量
	VECTOR GetPosPushBackAlongNormal(const MV1_COLL_RESULT_POLY& _hitColPoly,
									 int _maxTryCnt, float _pushDistance) const;


protected:

	/// @brief デバッグ描画
	void DrawDebug(unsigned int color) override;


private:

	// 親Transformからの相対位置(下側)
	VECTOR localPos_;

	// 半径
	float radius_;
};