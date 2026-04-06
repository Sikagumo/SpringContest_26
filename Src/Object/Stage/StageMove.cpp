#include "StageMove.h"
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Common/Vector2.h"
#include "../../Utility/AsoUtility.h"
#include "../StageObj/StageObjBase.h"
#include "../StageObj/StageObjWall.h"
#include "../StageObj/StageObjGoal.h"
#include "../StageObj/StageObjTrap.h"
#include "../StageObj/StageObjTrapVerticalMove.h"
#include "../StageObj/StageObjTrapHorizontalMove.h"


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

	BLOCK_TYPE objType = static_cast<BLOCK_TYPE>(_blockType);

	float scale = 1.0f;

	// 座標計算
	VECTOR pos = VGet((_x * (BLOCK_OFFSET.x * scale) + STAGE_POS.x),
					  (_y * (BLOCK_OFFSET.y * scale) + STAGE_POS.y),
					   STAGE_POS.z);

	// プレイヤー１登録
	if (objType == BLOCK_TYPE::PLAYER_WIDTH
		&& AsoUtility::EqualsVZero(playersPos_[0]))
	{
		playersPos_[0] = pos;
	}

	// プレイヤー２登録
	else if (objType == BLOCK_TYPE::PLAYER_HEIGHT
			&& stageType_ == TYPE::MOVE
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

		if (stageType_ == TYPE::MOVE)
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

		MV1SetMaterialSpcColor(ret->GetTransform().modelId, 0,
			COLOR_F(color.r, color.g, color.b,
				MV1GetMaterialSpcColor(ret->GetTransform().modelId, 0).a));

		MV1SetMaterialDifColor(ret->GetTransform().modelId, 0,
			COLOR_F(color.r, color.g, color.b,
				MV1GetMaterialDifColor(ret->GetTransform().modelId, 0).a));

		//トラップの座標をリストに保存する
		trapPositions_.push_back(ret->GetTransform().pos);
	}
	// 垂直移動トラップ登録
	else if (objType == BLOCK_TYPE::TRAP_VEERTICAL_MOVE)
	{
		auto* vTrap = new StageObjTrapVerticalMove(_x, _y, _blockType);
		vTrap->SetStage(this);
		vTrap->Init(pos);      // これを呼ばないと InitLoad が走らず描画されません
		ret = vTrap;

		trapPositions_.push_back(ret->GetTransform().pos);
	}
	//横移動トラップ登録
	else if (objType == BLOCK_TYPE::TRAP_HORIZONTAL_MOVE)
	{
		auto* hTrap = new StageObjTrapHorizontalMove(_x, _y, _blockType);
		hTrap->SetStage(this);
		hTrap->Init(pos);
		ret = hTrap;
		trapPositions_.push_back(ret->GetTransform().pos);
	}

	return ret;
}
StageObjBase* StageMove::SetParamBack(int _blockType, int _x, int _y, float _alpha, bool _isCollision)
{
	// 奥行がない場合はnullで返す
	if (stageType_ != TYPE::MOVE3D) { return nullptr; }

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
	//垂直移動トラップ登録
	else if (objType == BLOCK_TYPE::TRAP_VEERTICAL_MOVE) // 列挙型に追加した名前
	{
		// ステージのポインタを渡す必要がある場合は、コンストラクタやInitを調整
		auto* vTrap = new StageObjTrapVerticalMove(_x, _y, _blockType);
		vTrap->SetStage(this); // 壁判定のために自分(Stage)を教える
		vTrap->Init(pos);
		ret = vTrap;

		// ※動くトラップの場合、ここで push_back した pos は固定値になってしまいます。
		// 対策は下の「重要なポイント」を参照してください。
		trapPositions_.push_back(ret->GetTransform().pos);
	}
	//横移動トラップ登録
	else if (objType == BLOCK_TYPE::TRAP_HORIZONTAL_MOVE)
	{
		auto* hTrap = new StageObjTrapHorizontalMove(_x, _y, _blockType);
		hTrap->SetStage(this);
		hTrap->Init(pos);
		ret = hTrap;
		trapPositions_.push_back(ret->GetTransform().pos);
	}

	return ret;
}

void StageMove::Update(void)
{
	//親クラスの更新（もし親で何か共通処理があれば実行）
	StageBase::Update();

	//当たり判定用のトラップ座標リストを一旦空にする
	trapPositions_.clear();

	//表側の全オブジェクトをチェック
	for (auto& row : placeType_) // 行(vector)を取り出す
	{
		for (auto* obj : row) // 各行の中のオブジェクトを取り出す
		{
			if (obj == nullptr) continue;

			// オブジェクトを更新（動くトラップならここで移動する）
			obj->Update();

			// トップまたは垂直移動トラップなら最新座標をリストに追加
			int type = obj->GetObjType();
			if (type == static_cast<int>(BLOCK_TYPE::TRAP) ||
				type == static_cast<int>(BLOCK_TYPE::TRAP_VEERTICAL_MOVE)||
				type == static_cast<int>(BLOCK_TYPE::TRAP_HORIZONTAL_MOVE))
			{
				trapPositions_.push_back(obj->GetTransform().pos);
			}
		}
	}

	//裏側の全オブジェクトもチェック（3Dモード等の場合）
	for (auto& row : placeBackType_)
	{
		for (auto* obj : row)
		{
			if (obj == nullptr) continue;

			obj->Update();

			int type = obj->GetObjType();
			if (type == static_cast<int>(BLOCK_TYPE::TRAP) ||
				type == static_cast<int>(BLOCK_TYPE::TRAP_VEERTICAL_MOVE)||
				type == static_cast<int>(BLOCK_TYPE::TRAP_HORIZONTAL_MOVE))
			{
				trapPositions_.push_back(obj->GetTransform().pos);
			}
		}
	}
}