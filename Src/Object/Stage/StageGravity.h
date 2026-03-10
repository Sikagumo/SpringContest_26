#pragma once
#include "./StageBase.h"
#include <array>
#include <map>
#include "../../CSV/CsvManager.h"
#include "../../Common/Vector2.h"

class Transform;
class StageObjBase;

class StageGravity : public StageBase
{
public:
    enum class BLOCK_TYPE
    {
        BLANK = -1,    // 空白or未割当
        WALL = 0,      // 壁
        PLAYER_DOWN = 1, // 下重力プレイヤー（P1など）
        PLAYER_UP = 2,   // 上重力プレイヤー（P2など）
        GOAL = 3,        // ゴール
        MAX,
    };

    StageGravity(void);
    ~StageGravity(void) override = default;

    void DrawDebug(void) override;

protected:
    // 必要に応じて表示位置（Z軸など）をStageMoveとずらす場合はここを調整
    static constexpr VECTOR STAGE_POS = { -1000.0f, -1000.0f, 850.0f };
    static constexpr float BLOCK_SCALE = 1.0f;
    static constexpr float BLOCK_OFFSET_X = 200.0f;
    static constexpr float BLOCK_OFFSET_Y = 200.0f;

    void InitList(void) override;

    /// @brief ブロック状態割り当て
    StageObjBase* SetParam(int _blockType, float _posX, float _posY) override;
};