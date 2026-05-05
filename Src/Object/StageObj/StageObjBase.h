#pragma once
#include "../Actor/ActorBase.h"
#include "../../Common/Vector2.h"

class StageObjBase : public ActorBase
{
public:

	/// @brief ステージのオブジェクト
	/// @param _arrayPos ステージの配列位置
	/// @param _collisionSize 当たり判定サイズ
	/// @param _objType オブジェクトの種類
	/// @param _alpha モデルの透過率(0.0～1.0)
	StageObjBase(Vector2 _arrayPos, const VECTOR& _collisionSize, int _objType = -1, float _alpha = 1.0f);

	virtual ~StageObjBase(void)override = default;

	/// @brief 初期化処理
	/// @param _pos 位置
	void Initialize(const VECTOR& _pos);
	virtual void Update(void)override = 0;
	void Draw(void)override;


	const Vector2F& GetCollisionPos(void)const { return collisionPos_; };

	const Vector2F& GetCollisionSize(void)const { return collisionSize_; };

	const ColliderBase* GetOwnCollider(void);

	int GetObjType(void)const { return objType_; };

	/// @brief モデルの透過率割り当て(0.0～1.0)
	void SetAlpha(float _alpha);

	/// @brief オブジェクトのステージ位置取得
	const Vector2& GetObjArrayPos(void)const { return arrayPos_; };


protected:

	// オブジェクトの種類
	int objType_;

	// 当たり判定座標
	Vector2F collisionPos_;

	// 当たり判定サイズ
	Vector2F collisionSize_;

	// 配列位置
	Vector2 arrayPos_;

	// モデルの透過率(0.0～1.0)
	float alpha_;


	virtual void InitLoad(void)override = 0;
	virtual void InitTransform(void)override = 0;
	virtual void InitCollider(void)override = 0;
	virtual void InitAnimation(void) {};
	virtual void InitPost(void) {};
};