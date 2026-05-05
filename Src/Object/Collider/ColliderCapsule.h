#pragma once
#include <DxLib.h>
#include "ColliderBase.h"
class Transform;
class ColliderModel;

class ColliderCapsule : public ColliderBase
{
public:

	/// @brief コンストラクタ
	/// @param _tag 自身のカプセル
	/// @param _follow 追従対象
	/// @param _localPosTop カプセルの上位置
	/// @param _localPosDown カプセルの下位置
	/// @param _radius 半径
	ColliderCapsule(TAG _tag, const Transform* _follow,
					const VECTOR& _localPosTop, const VECTOR& _localPosDown, float _radius);

	~ColliderCapsule(void) = default;


	/* 親Transformからの相対位置を取得 */
	const VECTOR& GetLocalPosTop(void) const { return localPosTop_; };
	const VECTOR& GetLocalPosDown(void) const { return localPosDown_; };

	/* 親Transformからの相対位置を割り当て */
	void SetLocalPosTop(const VECTOR& _pos) { localPosTop_ = _pos; };
	void SetLocalPosDown(const VECTOR& _pos) { localPosDown_ = _pos; };


	/* ワールド座標を取得 */
	VECTOR GetPosTop(void) const;
	VECTOR GetPosDown(void) const;

	/// @brief 半径取得
	float GetRadius(void) const { return radius_; };

	/// @brief 半径割り当て
	void SetRadius(float _radius) { radius_ = _radius; };

	/// @brief カプセルの高さを取得
	float GetHeight(void) const { return localPosTop_.y; };

	/// @brief カプセルの中心座標
	VECTOR GetCenter(void) const;

	/// @brief 指定された回数と距離で三角形の法線方向に押し戻した座標を取得
	/// @param _hitColPoly 反発するポリゴン
	/// @param _maxTryCnt 最大反発数
	/// @param _pushDistance 反発量
	VECTOR GetPosPushBackAlongNormal(const MV1_COLL_RESULT_POLY& _hitColPoly,
									int _maxTryCnt, float _pushDistance) const;

	/// @brief 指定された回数と距離で三角形の法線方向に押し戻す
	/// @param _colliderModel 反発するモデル
	/// @param _transform 
	/// @param _maxTryCnt 最大反発数
	/// @param _pushDistance 反発量
	/// @param _isExclude タグ別に除外をするか否か
	/// @param _isTarget 対象フレームを除外するか否か
	void PushBackAlongNormal(const ColliderModel* _colliderModel, Transform* _transform,
							 int _maxTryCnt, float _pushDistance,
							 bool _isExclude = false, bool _isTarget = false) const;
	
	/// @brief モデルと衝突しているか否か
	/// @param _colliderModel 反発するモデル
	/// @param _isExcude タグ別に除外をするか否か
	/// @param _isTarget 対象フレームを除外するか否か
	bool IsHit(const ColliderModel* _colliderModel,
				bool _isExcude = false, bool _isTarget = false) const;


protected:

	/// @brief デバッグ描画
	/// @param _color カプセルの色
	void DrawDebug(unsigned int _color) override;


private:

	// 親Transformからの相対位置(上側)
	VECTOR localPosTop_;

	// 親Transformからの相対位置(下側)
	VECTOR localPosDown_;

	// 半径
	float radius_;
};
