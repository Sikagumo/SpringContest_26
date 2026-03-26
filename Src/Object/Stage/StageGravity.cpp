#include "StageGravity.h"
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Common/Vector2.h"
#include "../../Utility/AsoUtility.h"
#include "../StageObj/StageObjBase.h"
#include "../StageObj/StageObjWall.h"
#include "../StageObj/StageObjGoal.h"

StageGravity::StageGravity(bool _isBack)
    : StageBase::StageBase(((_isBack) ? TYPE::GRAVITY3D : TYPE::GRAVITY),
							CsvManager::GetInstance().GetStageGravityMapNum())
{
}

void StageGravity::DrawDebug(void)
{
}

StageObjBase* StageGravity::SetParam(int _blockType, int _x, int _y)
{
    StageObjBase* ret = nullptr;
    BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);
    float scale = BLOCK_SCALE;

    // ç¿ïWåvéZ
    VECTOR pos = VGet((_x * (BLOCK_OFFSET.x * scale) + STAGE_POS.x),
                      (_y * (BLOCK_OFFSET.y * scale) + STAGE_POS.y),
                      STAGE_POS.z);

    // ÉvÉåÉCÉÑÅ[1 (â∫èdóÕ) ìoò^
    if (type == BLOCK_TYPE::PLAYER_DOWN
		&& AsoUtility::EqualsVZero(playersPos_[0]))
    {
        playersPos_[0] = pos;
    }

    // ÉvÉåÉCÉÑÅ[2 (è„èdóÕ) ìoò^
    else if (type == BLOCK_TYPE::PLAYER_UP
			 && stageType_ == TYPE::MOVE
			 && AsoUtility::EqualsVZero(playersPos_[1]))
    {
        playersPos_[1] = pos;
    }

    // ÉSÅ[Éãìoò^
    else if (type == BLOCK_TYPE::GOAL)
    {
        ret = new StageObjGoal(_x, _y, _blockType);
        ret->Init(pos);
        goalPos_ = ret->GetTransform().pos;

		if (stageType_ == TYPE::GRAVITY)
		{
			goalPosBack_ = ret->GetTransform().pos;
		}
    }

    // ï«ìoò^
    else if (type == BLOCK_TYPE::WALL)
    {
        ret = new StageObjWall(_x, _y, _blockType);
        ret->Init(pos);
    }

    return ret;
}
StageObjBase* StageGravity::SetParamBack(int _blockType, int _x, int _y)
{
	// âúçsÇ™Ç»Ç¢èÍçáÇÕnullÇ≈ï‘Ç∑
	if (stageType_ != TYPE::GRAVITY3D) { return nullptr; }

	StageObjBase* ret = nullptr;

	BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);

	VECTOR pos = VGet((_x * (BLOCK_OFFSET.x * BLOCK_SCALE) + STAGE_POS.x),
					  (_y * (BLOCK_OFFSET.y * BLOCK_SCALE) + STAGE_POS.y),
					  (STAGE_POS.z + BLOCK_OFFSET.z));

	// ÉvÉåÉCÉÑÅ[ÇPìoò^
	if (type == BLOCK_TYPE::PLAYER_DOWN
		&& AsoUtility::EqualsVZero(playersPos_[0]))
	{
		playersPos_[0] = pos;
	}

	// ÉvÉåÉCÉÑÅ[ÇQìoò^
	else if (type == BLOCK_TYPE::PLAYER_UP
		&& AsoUtility::EqualsVZero(playersPos_[1]))
	{
		playersPos_[1] = pos;
	}

	// ÉSÅ[Éãìoò^
	else if (type == BLOCK_TYPE::GOAL)
	{
		ret = new StageObjGoal(_x, _y, _blockType);
		ret->Init(pos);
		goalPosBack_ = ret->GetTransform().pos;
	}

	// ï«ìoò^
	else if (type == BLOCK_TYPE::WALL)
	{
		ret = new StageObjWall(_x, _y, _blockType);
		ret->Init(pos);
	}

	// ñ¢äÑìñéûÅAìßâﬂÉIÉuÉWÉFÉNÉgÇÃí«â¡
	if (ret == nullptr)
	{
		ret = new StageObjWall(_x, _y, _blockType, BACK_ALPHA, false);
		ret->Init(pos);
	}

	return ret;
}
