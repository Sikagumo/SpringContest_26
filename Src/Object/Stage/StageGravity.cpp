#include "StageGravity.h"
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Common/Vector2.h"
#include "../../Utility/AsoUtility.h"
#include "../StageObj/StageObjBase.h"
#include "../StageObj/StageObjWall.h"
#include "../StageObj/StageObjGoal.h"

StageGravity::StageGravity(void)
    : StageBase::StageBase(TYPE::GRAVITY, CsvManager::GetInstance().GetStageGravityMapNum())
{
}

void StageGravity::InitList(void)
{
    // CsvManagerから重力ステージ用の最大マップ数を取得してランダム決定
    int rand = GetRand(mapNumMax_ - 1);
    SetBlockTypeList(rand, CsvManager::STAGE_GRAVITY_X, CsvManager::STAGE_GRAVITY_Y);
}

void StageGravity::DrawDebug(void)
{
    // 必要であればデバッグ描画を記述
}

StageObjBase* StageGravity::SetParam(int _blockType, float _posX, float _posY)
{
    StageObjBase* ret = nullptr;
    BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);
    float scale = BLOCK_SCALE;

    // 座標計算
    VECTOR pos = VGet((_posX * (BLOCK_OFFSET_X * scale) + STAGE_POS.x),
        (_posY * (BLOCK_OFFSET_Y * scale) + STAGE_POS.y),
        STAGE_POS.z);

    // プレイヤー1 (下重力) 登録
    if (type == BLOCK_TYPE::PLAYER_DOWN)
    {
        playersPos_[0] = pos;
    }
    // プレイヤー2 (上重力) 登録
    else if (type == BLOCK_TYPE::PLAYER_UP)
    {
        playersPos_[1] = pos;
    }
    // ゴール登録
    else if (type == BLOCK_TYPE::GOAL)
    {
        ret = new StageObjGoal(_blockType);
        ret->Init(pos);
        goalPos_ = ret->GetTransform().pos;
    }
    // 壁登録
    else if (type == BLOCK_TYPE::WALL)
    {
        ret = new StageObjWall(_blockType);
        ret->Init(pos);
    }

    return ret;
}