#pragma once
#include "./StageObjBase.h"
#include <DxLib.h>
class StageBase;

class StageObjTrapVerticalMove : public StageObjBase
{
public:

	static constexpr float COLLISION_RADIUS = 10.0f;

	StageObjTrapVerticalMove(int _x, int _y, int _objType, float _alpha = 1.0f);

	// 空の {} ではなく、cpp側で移動ロジックを書くために宣言のみにする
	void Update(void)override;

	//ステージのポインタを受け取るための関数を追加
	void SetStage(StageBase* _stage) { stage_ = _stage; }
protected:

	static constexpr float BLOCK_SCALE = 0.15f;
	static constexpr float COLLISION_SIZE_X = 10.0f;
	static constexpr float COLLISION_SIZE_Y = 10.0f;
	static constexpr VECTOR MODEL_OFFSET = { 0.0f, 0.0f, 0.0f };

	StageBase* stage_ = nullptr;
	float moveSpeed_ = 2.0f;
	int moveDir_ = 1;

	void InitLoad(void)override;

	void InitTransform(void)override;

	void InitCollider(void)override;

};

