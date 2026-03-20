#pragma once
#include "../Actor/ActorBase.h"
#include "../../Common/Vector2F.h"
#include "../../Common/Vector2.h"

class StageObjBase : public ActorBase
{
public:

	/// @brief ステージのオブジェクト
	/// @param _pos オブジェクト位置
	/// @param _collisionSize 当たり判定サイズ
	/// @param _objType オブジェクトの種類
	/// @param _alpha モデルの透過率(0.0～1.0)
	StageObjBase(const VECTOR& _collisionSize, int _objType = -1, float _alpha = 1.0f);

	virtual ~StageObjBase(void)override = default;

	void Init(const VECTOR& _pos);

	/// @brief 更新処理
	virtual void Update(void)override = 0;

	void Draw(void)override;


	const Vector2F& GetCollisionPos(void)const { return collisionPos_; };

	const Vector2F& GetCollisionSize(void)const { return collisionSize_; };

	const ColliderBase* GetOwnCollider(void);

	int GetObjType(void) { return type; };

	void SetAlpha(float _alpha);


protected:

	// オブジェクトの種類
	int type;

	// 当たり判定座標
	Vector2F collisionPos_;

	// 当たり判定サイズ
	Vector2F collisionSize_;

	float alpha_;

	virtual void InitLoad(void)override = 0;
	virtual void InitTransform(void)override = 0;
	virtual void InitCollider(void)override = 0;
	virtual void InitAnimation(void) {};
	virtual void InitPost(void) {};
};