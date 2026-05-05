#pragma once
#include "StageObjBase.h"
#include "../../Common/Vector2.h"
class StageBase;

/// @brief 横移動トラップクラス
class StageObjTrapHorizontalMove : public StageObjBase
{
public:

    StageObjTrapHorizontalMove(Vector2 _arrayPos, int _objType, float _alpha = 1.0f);
    virtual ~StageObjTrapHorizontalMove(void) = default;

    /// @brief 初期化
    virtual void InitLoad(void) override;
    virtual void InitTransform(void) override;
    virtual void InitCollider(void) override;

    /// @brief 更新
    virtual void Update(void) override;

    //ステージのポインタを受け取るための関数を追加
    void SetStage(StageBase* _stage) { stage_ = _stage; }

private:
    // 定数設定
    static constexpr float COLLISION_RADIUS = 40.0f;
    static constexpr float BLOCK_SCALE = 0.15f;
    static constexpr float COLLISION_SIZE = 10.0f;

    static constexpr VECTOR MODEL_OFFSET = { 0.0f, 0.0f, 0.0f };

    StageBase* stage_ = nullptr;

    // 横移動用のパラメータ
    float moveSpeed_ = 2.5f;  // 移動速度
    int moveDir_ = 1;         // 移動方向 (1: 右 / -1: 左)
};