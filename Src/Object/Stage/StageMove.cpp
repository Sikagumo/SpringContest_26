#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Common/Vector2.h"
#include "../../Utility/AsoUtility.h"
#include "../StageObj/StageObjBase.h"
#include "../StageObj/StageObjWall.h"
#include "../StageObj/StageObjGoal.h"
#include "StageMove.h"

StageMove::StageMove(void)
	:StageBase::StageBase(TYPE::MOVE, CsvManager::GetInstance().GetStageMoveMapNum())
{

}

void StageMove::InitList(void)
{
	int rand = GetRand(mapNumMax_ - 1);
	SetBlockTypeList(rand, CsvManager::STAGE_MOVE_X, CsvManager::STAGE_MOVE_Y);
}

void StageMove::DrawDebug(void)
{
}


StageObjBase* StageMove::SetParam(int _blockType, float _posX, float _posY)
{
	StageObjBase* ret = nullptr;

	BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);

	float scale = 1.0f;

	VECTOR pos = VGet((_posX * (BLOCK_OFFSET_X * scale) + STAGE_POS.x),
					  (_posY * (BLOCK_OFFSET_Y * scale) + STAGE_POS.y),
					   STAGE_POS.z);

	// ƒvƒŒƒCƒ„[‚P“o˜^
	if (type == BLOCK_TYPE::PLATER_WIDTH)
	{
		playersPos_[0] = pos;
	}

	// ƒvƒŒƒCƒ„[‚Q“o˜^
	else if (type == BLOCK_TYPE::PLATER_HEIGHT)
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