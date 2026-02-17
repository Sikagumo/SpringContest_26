#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Common/Vector2.h"
#include "../../Utility/AsoUtility.h"
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

	float scale = 1.0f;
	
	BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);
	Quaternion rotLocal = Quaternion::Identity();
	VECTOR pos = VGet((_posX * (BLOCK_OFFSET_X * scale) + STAGE_POS.x),
					  (_posY * (BLOCK_OFFSET_Y * scale) + STAGE_POS.y),
					   STAGE_POS.z);

	if (type == BLOCK_TYPE::WALL)
	{
		scale = BLOCK_SCALE;
		_param.viewParam->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE_STONE));
	}
	else if (type == BLOCK_TYPE::GOAL)
	{
		// ゴール登録
		pos.x += 25.0f;
		pos.y += 10.0f;
		goalPos_ = pos;

		scale = 0.15f;
		rotLocal = Quaternion::AngleAxis(-90.0f, AsoUtility::AXIS_X);
		_param.viewParam->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_GOAL));
	}
	else if (type == BLOCK_TYPE::PLATER_WIDTH)
	{
		// プレイヤー１登録
		playersPos_[0] = pos;
		_param.viewParam->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE_BLANK));
	}
	else if (type == BLOCK_TYPE::PLATER_HEIGHT)
	{
		// プレイヤー２登録
		playersPos_[1] = pos;
		_param.viewParam->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE_BLANK));
	}
	else
	{
		_param.viewParam->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE_BLANK));
	}

	_param.viewParam->InitTransform(scale,
									Quaternion::Identity(), rotLocal,
									pos);

	
	// ブロックの当たり判定位置
	_param.collisionPosX = _param.viewParam->pos.x;
	_param.collisionPosY = _param.viewParam->pos.y;

	// ブロックの当たり判定のサイズ
	_param.collisionSize = Vector2(BLOCK_SCALE, BLOCK_SCALE);
}