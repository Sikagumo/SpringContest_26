#include "StageGravity.h"
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Common/Vector2.h"
#include "../../Utility/AsoUtility.h"
#include "../StageObj/StageObjBase.h"
#include "../StageObj/StageObjWall.h"
#include "../StageObj/StageObjGoal.h"
#include "../StageObj/StageObjTrap.h"

StageGravity::StageGravity(bool _isBack)
    : StageBase::StageBase(((_isBack) ? TYPE::GRAVITY3D : TYPE::GRAVITY)
    					   , CsvManager::GetInstance().GetStageGravityMapNum())
{
}

void StageGravity::DrawDebug(void)
{
}

StageObjBase* StageGravity::SetParam(int _blockType, int _x, int _y)
{
    StageObjBase* ret = nullptr;
    BLOCK_TYPE objType = static_cast<BLOCK_TYPE>(_blockType);
    float scale = BLOCK_SCALE;

    // 座標計算
    VECTOR pos = VGet((_x * (BLOCK_OFFSET.x * scale) + STAGE_POS.x),
                      (_y * (BLOCK_OFFSET.y * scale) + STAGE_POS.y),
                      STAGE_POS.z);

    // プレイヤー1 (下重力) 登録
    if (objType == BLOCK_TYPE::PLAYER_WIDTH
		&& AsoUtility::EqualsVZero(playersPos_[0]))
    {
        playersPos_[0] = pos;
    }

    // プレイヤー2 (上重力) 登録
    else if (objType == BLOCK_TYPE::PLAYER_HEIGHT
			 && stageType_ == TYPE::GRAVITY
			 && AsoUtility::EqualsVZero(playersPos_[1]))
    {
        playersPos_[1] = pos;
    }

    // ゴール登録
    else if (objType == BLOCK_TYPE::GOAL)
    {
        ret = new StageObjGoal(_x, _y, _blockType);
        ret->Init(pos);
        goalPos_ = ret->GetTransform().pos;

		if (stageType_ == TYPE::GRAVITY)
		{
			goalPosBack_ = ret->GetTransform().pos;
		}
    }

    // 壁登録
    else if (objType == BLOCK_TYPE::WALL)
    {
        ret = new StageObjWall(_x, _y, _blockType);
        ret->Init(pos);

		COLOR_F color = UtilityCommon::GetColorRate(WALL_COLOR_FLONT);

		MV1SetMaterialDifColor(ret->GetTransform().modelId, 0,
			COLOR_F(color.r, color.g, color.b,
				MV1GetMaterialDifColor(ret->GetTransform().modelId, 0).a));
    }
    //トラップ登録
    else if (objType == BLOCK_TYPE::TRAP)
    {
        ret = new StageObjTrap(_x, _y, _blockType);
        ret->Init(pos);
        
		COLOR_F color = UtilityCommon::GetColorRate(WALL_COLOR_FLONT);

		MV1SetMaterialDifColor(ret->GetTransform().modelId, 0,
			COLOR_F(color.r, color.g, color.b,
				MV1GetMaterialDifColor(ret->GetTransform().modelId, 0).a));

        //トラップの座標をリストに保存する
        trapPositions_.push_back(ret->GetTransform().pos);
    }

    return ret;
}
StageObjBase* StageGravity::SetParamBack(int _blockType, int _x, int _y, float _alpha, bool _isCollision)
{
	// 奥行がない場合はnullで返す
	if (stageType_ != TYPE::GRAVITY3D) { return nullptr; }

	StageObjBase* ret = nullptr;

	BLOCK_TYPE objType = static_cast<BLOCK_TYPE>(_blockType);

	VECTOR pos = VGet((_x * (BLOCK_OFFSET.x * BLOCK_SCALE) + STAGE_POS.x),
					  (_y * (BLOCK_OFFSET.y * BLOCK_SCALE) + STAGE_POS.y),
					  (STAGE_POS.z + BLOCK_OFFSET.z));


	// プレイヤー２登録
	if (objType == BLOCK_TYPE::PLAYER_HEIGHT
		&& AsoUtility::EqualsVZero(playersPos_[1]))
	{
		pos.z += PLAYER_OFFSET_Z;
		playersPos_[1] = pos;
	}

	// ゴール登録
	else if (objType == BLOCK_TYPE::GOAL)
	{
		ret = new StageObjGoal(_x, _y, _blockType);
		ret->Init(pos);
		goalPosBack_ = ret->GetTransform().pos;
	}

	// 壁登録
	else if (objType == BLOCK_TYPE::WALL)
	{
		ret = new StageObjWall(_x, _y, _blockType, _alpha, _isCollision);
		ret->Init(pos);
		
		COLOR_F color = UtilityCommon::GetColorRate(WALL_COLOR_BACK);

		MV1SetMaterialDifColor(ret->GetTransform().modelId, 0,
			COLOR_F(color.r, color.g, color.b,
				MV1GetMaterialDifColor(ret->GetTransform().modelId, 0).a));
	}

	//トラップ登録
	else if (objType == BLOCK_TYPE::TRAP)
	{
		ret = new StageObjTrap(_x, _y, _blockType);
		ret->Init(pos);

		//トラップの座標をリストに保存する
		trapPositions_.push_back(ret->GetTransform().pos);
	}

	// 未割当時、透過オブジェクトの追加
	if (ret == nullptr)
	{
		ret = new StageObjWall(_x, _y, _blockType, BACK_ALPHA, false);
		ret->Init(pos);

		COLOR_F color = UtilityCommon::GetColorRate(WALL_COLOR_BACK);

		MV1SetMaterialDifColor(ret->GetTransform().modelId, 0,
							   COLOR_F(color.r, color.g, color.b, 
									   MV1GetMaterialDifColor(ret->GetTransform().modelId, 0).a));
	}

	return ret;
}
