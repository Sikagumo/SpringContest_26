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

    StageGravity(bool _isBack);

    ~StageGravity(void) override = default;

    void DrawDebug(void) override;

    void Update(void) override; 
protected:


    /// @brief ƒuƒƒbƒNó‘ÔŠ„‚è“–‚Ä
    StageObjBase* SetParam(int _blockType, int _x, int _y) override;
    StageObjBase* SetParamBack(int _blockType, int _x, int _y, float _alpha = 1.0f, bool _isCollision = true)override;
};