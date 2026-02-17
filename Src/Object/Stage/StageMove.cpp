#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Common/Vector2.h"
#include "../Common/Transform.h"
#include "StageMove.h"

StageMove::StageMove(void)
	:StageBase::StageBase()
{

}

void StageMove::InitList(void)
{
	SetBlockTypeList(0, CsvManager::STAGE_MOVE_X, CsvManager::STAGE_MOVE_Y);
}

void StageMove::DrawDebug(void)
{
	/*
	const int x = 0;
	int y = 16*3;
	for (auto placeList : placeType_)
	{
		for (auto& place : placeList)
		{
			DrawFormatString(x, y, 0xffffff, "obj:(view[%.1f, %.1f, %.1f], colPos[%d, %d])",
							 place->viewParam->pos.x,
							 place->viewParam->pos.y,
							 place->viewParam->pos.z,
						     place->collisionPosX, place->collisionPosY);
			y += 16;
		}
	}*/
}


void StageMove::SetParam(BlockParam& _param, int _blockType, float _posX, float _posY)
{
	_param.type = _blockType;
	_param.viewParam = new Transform();

	_param.viewParam->InitTransform(BLOCK_SCALE,
		Quaternion::Identity(), Quaternion::Identity(),
		{ (_posX * (BLOCK_OFFSET_X * BLOCK_SCALE) + STAGE_POS.x),
		  (_posY * (BLOCK_OFFSET_Y * BLOCK_SCALE) + STAGE_POS.y),
		  STAGE_POS.z
		}
	);
	
	BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);
	if (type == BLOCK_TYPE::WALL)
	{
		_param.viewParam->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE_STONE));
	}
	else
	{
		_param.viewParam->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE_BLANK));
	}

	
	// ブロックの当たり判定位置
	_param.collisionPosX = _param.viewParam->pos.x;
	_param.collisionPosY = _param.viewParam->pos.y;

	// ブロックの当たり判定のサイズ
	_param.collisionSize = Vector2(BLOCK_SCALE, BLOCK_SCALE);
}
