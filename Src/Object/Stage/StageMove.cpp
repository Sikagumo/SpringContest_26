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

void StageMove::DrawDebug(void)
{
}


StageObjBase* StageMove::SetParam(int _blockType, float _posX, float _posY)
{
	StageObjBase* ret = nullptr;

	BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);

	float scale = 1.0f;

	VECTOR pos = VGet((_posX * (BLOCK_OFFSET.x * scale) + STAGE_POS.x),
					  (_posY * (BLOCK_OFFSET.y * scale) + STAGE_POS.y),
					   STAGE_POS.z + BLOCK_OFFSET.z);

	// ÉvÉåÉCÉÑÅ[ÇPìoò^
	if (type == BLOCK_TYPE::PLATER_WIDTH)
	{
		playersPos_[0] = pos;
	}

	// ÉvÉåÉCÉÑÅ[ÇQìoò^
	else if (type == BLOCK_TYPE::PLATER_HEIGHT
			&& stageType_ == TYPE::MOVE)
	{
		playersPos_[1] = pos;
	}

	// ÉSÅ[Éãìoò^
	else if (type == BLOCK_TYPE::GOAL)
	{
		ret = new StageObjGoal(_blockType);
		ret->Init(pos);
		goalPos_ = ret->GetTransform().pos;
	}

	// ï«ìoò^
	else if (type == BLOCK_TYPE::WALL)
	{
		ret = new StageObjWall(_blockType);
		ret->Init(pos);
	}

	return ret;
}
StageObjBase* StageMove::SetParamBack(int _blockType, float _posX, float _posY)
{
	// âúçsÇ™Ç»Ç¢èÍçáÇÕnullÇ≈ï‘Ç∑
	if (stageType_ != TYPE::MOVE3D) { return nullptr; }

	StageObjBase* ret = nullptr;

	BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);

	float scale = 1.0f;

	VECTOR pos = VGet((_posX * (BLOCK_OFFSET.x * scale) + STAGE_POS.x),
					  (_posY * (BLOCK_OFFSET.y * scale) + STAGE_POS.y),
					  STAGE_POS.z);

	// ÉvÉåÉCÉÑÅ[ÇPìoò^
	if (type == BLOCK_TYPE::PLATER_WIDTH &&
		stageType_ == TYPE::MOVE3D)
	{
		playersPos_[0] = pos;
	}

	// ÉvÉåÉCÉÑÅ[ÇQìoò^
	else if (type == BLOCK_TYPE::PLATER_HEIGHT)
	{
		playersPos_[1] = pos;
	}

	// ÉSÅ[Éãìoò^
	else if (type == BLOCK_TYPE::GOAL)
	{
		ret = new StageObjGoal(_blockType);
		ret->Init(pos);
		goalPos_ = ret->GetTransform().pos;
	}

	// ï«ìoò^
	else if (type == BLOCK_TYPE::WALL)
	{
		ret = new StageObjWall(_blockType);
		ret->Init(pos);
	}

	return ret;
}
