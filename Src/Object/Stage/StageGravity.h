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
        BLANK = -1,  // 空白or未割当
        WALL,        // 壁
        PLAYER_DOWN, // 下重力プレイヤー（P1など）
        PLAYER_UP,   // 上重力プレイヤー（P2など）

        GOAL,        // ゴール

        MAX,
    };

    StageGravity(bool _isBack);

    ~StageGravity(void) override = default;

    void DrawDebug(void) override;


protected:


    /// @brief ブロック状態割り当て
    StageObjBase* SetParam(int _blockType, int _x, int _y) override;
    StageObjBase* SetParamBack(int _blockType, int _x, int _y)override;
};