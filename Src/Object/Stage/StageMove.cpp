#include "StageMove.h"
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Common/Vector2.h"
#include "../../Utility/AsoUtility.h"
#include "../StageObj/StageObjBase.h"
#include "../StageObj/StageObjWall.h"
#include "../StageObj/StageObjGoal.h"
#include "../StageObj/StageObjTrap.h"

StageMove::StageMove(bool _isBack)
	: StageBase::StageBase(((_isBack) ? TYPE::MOVE3D : TYPE::MOVE)
						   , CsvManager::GetInstance().GetStageMoveMapNum())
{

}

void StageMove::DrawDebug(void)
{
}


StageObjBase* StageMove::SetParam(int _blockType, int _x, int _y)
{
	StageObjBase* ret = nullptr;

	BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);

	float scale = 1.0f;

	// ç¿ïWåvéZ
	VECTOR pos = VGet((_x * (BLOCK_OFFSET.x * scale) + STAGE_POS.x),
					  (_y * (BLOCK_OFFSET.y * scale) + STAGE_POS.y),
					   STAGE_POS.z);

	// ÉvÉåÉCÉÑÅ[ÇPìoò^
	if (type == BLOCK_TYPE::PLATER_WIDTH
		&& AsoUtility::EqualsVZero(playersPos_[0]))
	{
		playersPos_[0] = pos;
	}

	// ÉvÉåÉCÉÑÅ[ÇQìoò^
	else if (type == BLOCK_TYPE::PLATER_HEIGHT
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

		if (stageType_ == TYPE::MOVE)
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

	//ÉgÉâÉbÉvìoò^
	else if (type == BLOCK_TYPE::TRAP)
	{
		ret = new StageObjTrap(_x, _y, _blockType);
		ret->Init(pos);

		//ÉgÉâÉbÉvÇÃç¿ïWÇÉäÉXÉgÇ…ï€ë∂Ç∑ÇÈ
		trapPositions_.push_back(ret->GetTransform().pos);
	}

	return ret;
}
StageObjBase* StageMove::SetParamBack(int _blockType, int _x, int _y)
{
	// âúçsÇ™Ç»Ç¢èÍçáÇÕnullÇ≈ï‘Ç∑
	if (stageType_ != TYPE::MOVE3D) { return nullptr; }

	StageObjBase* ret = nullptr;

	BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);
	
	VECTOR pos = VGet((_x * (BLOCK_OFFSET.x * BLOCK_SCALE) + STAGE_POS.x),
					  (_y * (BLOCK_OFFSET.y * BLOCK_SCALE) + STAGE_POS.y),
					  (STAGE_POS.z + BLOCK_OFFSET.z));

	// ÉvÉåÉCÉÑÅ[ÇPìoò^
	if (type == BLOCK_TYPE::PLATER_WIDTH
		&& AsoUtility::EqualsVZero(playersPos_[0]))
	{
		pos.z += PLAYER_OFFSET_Z;
		playersPos_[0] = pos;
	}

	// ÉvÉåÉCÉÑÅ[ÇQìoò^
	else if (type == BLOCK_TYPE::PLATER_HEIGHT
			 && AsoUtility::EqualsVZero(playersPos_[1]))
	{
		pos.z += PLAYER_OFFSET_Z;
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

	//ÉgÉâÉbÉvìoò^
	else if (type == BLOCK_TYPE::TRAP)
	{
		ret = new StageObjTrap(_x, _y, _blockType);
		ret->Init(pos);

		//ÉgÉâÉbÉvÇÃç¿ïWÇÉäÉXÉgÇ…ï€ë∂Ç∑ÇÈ
		trapPositions_.push_back(ret->GetTransform().pos);
	}

	// ñ¢äÑìñéûÅAìßâﬂÉIÉuÉWÉFÉNÉgÇÃí«â¡
	if (ret == nullptr)
	{
		ret = new StageObjWall(_x, _y, _blockType, BACK_ALPHA, false);
		ret->Init(pos);
	}

	return ret;
}
