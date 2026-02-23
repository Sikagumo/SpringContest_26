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
	return;
	const int x = 0;
	int y = (16 * 3);
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
	}
}


StageBase::BlockParam* StageMove::SetParam(int _blockType, float _posX, float _posY)
{
	StageBase::BlockParam* ret = nullptr;

	BLOCK_TYPE type = static_cast<BLOCK_TYPE>(_blockType);

	float scale = 1.0f;

	Quaternion rotLocal = Quaternion::Identity();
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
	else
	{
		ret = new BlockParam();
		ret->type = _blockType;
		ret->viewParam = new Transform();

		// ƒS[ƒ‹“o˜^
		if (type == BLOCK_TYPE::GOAL)
		{
			pos.x += 25.0f;
			pos.y += 10.0f;

			goalPos_ = pos;


			scale = 0.15f;
			rotLocal = Quaternion::AngleAxis(-90.0f, AsoUtility::AXIS_X);
			ret->viewParam->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_GOAL));
		}

		else
		{
			// •Ç“o˜^
			if (type == BLOCK_TYPE::WALL)
			{
				scale = BLOCK_SCALE;
				ret->viewParam->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE_STONE));
			}

			// —áŠO“o˜^
			else
			{
				ret->viewParam->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE_BLANK));
			}
		}

		ret->viewParam->InitTransform(scale,
			Quaternion::Identity(), rotLocal,
			pos);

		// ƒuƒƒbƒN‚Ì“–‚½‚è”»’èˆÊ’u
		ret->collisionPosX = pos.x;
		ret->collisionPosY = pos.y;

		// ƒuƒƒbƒN‚Ì“–‚½‚è”»’è‚ÌƒTƒCƒY
		ret->collisionSize = Vector2(BLOCK_SCALE, BLOCK_SCALE);
	}

	return ret;
}