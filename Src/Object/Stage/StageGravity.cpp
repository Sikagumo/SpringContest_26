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

void StageGravity::DrawDebug(void)
{
    // •K—v‚Å‚ ‚ê‚ÎƒfƒoƒbƒO•`‰æ‚ğ‹Lq
}

StageObjBase* StageGravity::SetParam(int _blockType, float _posX, float _posY)
{
    StageObjBase* ret = nullptr;
    BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);
    float scale = BLOCK_SCALE;

    // À•WŒvZ
    VECTOR pos = VGet((_posX * (BLOCK_OFFSET.x * scale) + STAGE_POS.x),
                      (_posY * (BLOCK_OFFSET.y * scale) + STAGE_POS.y),
                      STAGE_POS.z);

    // ƒvƒŒƒCƒ„[1 (‰ºd—Í) “o˜^
    if (type == BLOCK_TYPE::PLAYER_DOWN)
    {
        playersPos_[0] = pos;
    }

    // ƒvƒŒƒCƒ„[2 (ãd—Í) “o˜^
    else if (type == BLOCK_TYPE::PLAYER_UP)
    {
        playersPos_[1] = pos;
    }

    // ƒS[ƒ‹“o˜^
    else if (type == BLOCK_TYPE::GOAL)
    {
        ret = new StageObjGoal(_blockType);
        ret->Init(pos);
        goalPos_ = ret->GetTransform().pos;
    }

    // •Ç“o˜^
    else if (type == BLOCK_TYPE::WALL)
    {
        ret = new StageObjWall(_blockType);
        ret->Init(pos);
    }

    return ret;
}
StageObjBase* StageGravity::SetParamBack(int _blockType, float _posX, float _posY)
{
	// ‰œs‚ª‚È‚¢ê‡‚Ínull‚Å•Ô‚·
	if (stageType_ != TYPE::MOVE3D) { return nullptr; }

	StageObjBase* ret = nullptr;

	BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);

	VECTOR pos = VGet((_posX * (BLOCK_OFFSET.x * BLOCK_SCALE) + STAGE_POS.x),
					  (_posY * (BLOCK_OFFSET.y * BLOCK_SCALE) + STAGE_POS.y),
					  (STAGE_POS.z + BLOCK_OFFSET.z));

	// ƒvƒŒƒCƒ„[‚P“o˜^
	if (type == BLOCK_TYPE::PLAYER_DOWN
		&& AsoUtility::EqualsVZero(playersPos_[0]))
	{
		playersPos_[0] = pos;
	}

	// ƒvƒŒƒCƒ„[‚Q“o˜^
	else if (type == BLOCK_TYPE::PLAYER_DOWN
		&& AsoUtility::EqualsVZero(playersPos_[1]))
	{
		playersPos_[1] = pos;
	}

	// ƒS[ƒ‹“o˜^
	else if (type == BLOCK_TYPE::GOAL)
	{
		ret = new StageObjGoal(_blockType);
		ret->Init(pos);
		goalPosBack_ = ret->GetTransform().pos;
	}

	// •Ç“o˜^
	else if (type == BLOCK_TYPE::WALL)
	{
		ret = new StageObjWall(_blockType);
		ret->Init(pos);
	}

	return ret;
}
