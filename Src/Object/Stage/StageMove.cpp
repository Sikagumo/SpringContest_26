#include <DxLib.h>
#include "../../CSV/CsvManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Common/Vector2.h"
#include "../Common/Transform.h"
#include "StageMove.h"

StageMove::StageMove(void)
	:StageBase::StageBase()
{

}

void StageMove::Init(void)
{
	for (int y = 0; y < CsvManager::STAGE_MOVE_Y; y++)
	{
		placeType_.emplace_back(SetBlockType(0, y));
	}

}

void StageMove::Update(void)
{
	for (auto placeList : placeType_)
	{
		for (auto& place : placeList)
		{
			place->viewParam->Update();
		}
	}
}

void StageMove::Draw(void)
{
	for (auto placeList : placeType_)
	{
		for (auto& place : placeList)
		{
			MV1DrawModel(place->viewParam->modelId);
		}
	}
}

void StageMove::DrawDebug(void)
{
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
						     place->collisionPos.x, place->collisionPos.y);
			y += 16;
		}
	}
}

void StageMove::Release(void)
{
	for (auto placeList : placeType_)
	{
		for (auto place : placeList)
		{
			place->viewParam->Release();
			delete place->viewParam;
		}
	}
}

std::vector<StageMove::BlockParam*> StageMove::SetBlockType(int _type, int line)
{
	std::vector<StageMove::BlockParam*> list;

	for (int x = 0; x < CsvManager::STAGE_MOVE_X; x++)
	{
		int type = CsvManager::GetInstance().GetStageMoveNum(_type, x, line);
		BLOCK_TYPE blockType = static_cast<BLOCK_TYPE>(type);

		// 要素以外の値の時、処理終了
		if (type <= -1 || type >= static_cast<int>(BLOCK_TYPE::MAX)) { continue; }

		BlockParam* param = new BlockParam();

		param->type = blockType;
		param->viewParam = new Transform();

		if (blockType == BLOCK_TYPE::WALL)
		{
			param->viewParam->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE_STONE));
		}
		else
		{
			param->viewParam->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE_BLANK));
		}

		param->viewParam->InitTransform(BLOCK_SCALE,
			Quaternion::Identity(), Quaternion::Identity(),
			{ (x * (BLOCK_OFFSET_X * BLOCK_SCALE) + STAGE_POS.x),
			  (line * (BLOCK_OFFSET_Y * BLOCK_SCALE) + STAGE_POS.y),
			  STAGE_POS.z
			}
		);

		param->collisionPos = Vector2(static_cast<int>(param->viewParam->pos.x),
									 static_cast<int>(param->viewParam->pos.y));

		param->collisionSize = Vector2(BLOCK_SCALE, BLOCK_SCALE);

		list.emplace_back(param);
	}

	return list;
}
