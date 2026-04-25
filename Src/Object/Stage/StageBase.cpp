#include "StageBase.h"
#include <cassert>
#include <Dxlib.h>
#include <array>
#include "../Actor/ActorBase.h"
#include "../../CSV/CsvManager.h"
#include "../StageObj/StageObjBase.h"
#include "../StageObj/StageObjWall.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Common/Vector2.h"
#include "../../Utility/AsoUtility.h"
#include "../Common/Transform.h"


StageBase::StageBase(TYPE stageType, int mapNum, int mapBackNum)
	: resMng_(ResourceManager::GetInstance())
	,sceneMng_(SceneManager::GetInstance())
	,csvMng_(CsvManager::GetInstance())
	,curStageType_(stageType), curStageNum_(-1)
	,mapNumMax_(mapNum), mapBackNumMax_(mapBackNum)
	,goalPos_(AsoUtility::VECTOR_ZERO), goalPosBack_(AsoUtility::VECTOR_ZERO)
{
	placeFrontList_.clear();
	placeBackList_.clear();
	placeBackBlankList_.clear();
	backGroundList_.clear();
	trapPositions_.clear();
	initialPlayersPos_.fill(AsoUtility::VECTOR_ZERO);
}

void StageBase::Init(int _stageNum)
{
	for (VECTOR& pos : initialPlayersPos_)
	{
		pos = AsoUtility::VECTOR_ZERO;
	}
	goalPos_ = goalPosBack_ = AsoUtility::VECTOR_ZERO;

	placeFrontList_.clear();
	placeBackList_.clear();
	placeBackBlankList_.clear();
	backGroundList_.clear();
	trapPositions_.clear();
	initialPlayersPos_.fill(AsoUtility::VECTOR_ZERO);

	// ステージ割り当て
	StageChoice(_stageNum);
	
}
void StageBase::StageChoice(int _stageNum)
{
	curStageNum_ = _stageNum;

	// リスト外の場合、０番を読み込む
	if (curStageNum_ >= (mapNumMax_ - 1)
		|| curStageNum_ < 0)
	{ curStageNum_ = 0; }


	// ステージ配置処理
	SetBlockTypeList(curStageNum_, CsvManager::STAGE_X, CsvManager::STAGE_Y);

	if (curStageType_ == TYPE::MOVE3D ||
		curStageType_ == TYPE::GRAVITY3D)
	{
		// ステージ奥配置処理
		SetBlockBackTypeList(curStageNum_, CsvManager::STAGE_X, CsvManager::STAGE_Y);

		// 前ステージの透過
		ChangeFrontObjects();
	}

	// 背景用配置処理
	SetBackGroundList(CsvManager::STAGE_X, CsvManager::STAGE_Y);
}

void StageBase::Draw(void)
{
	// 設置オブジェクト描画
	for (auto& placeBackList : placeBackList_)
	{
		for (auto& back : placeBackList)
		{
			bool isTrap = false;

			for (auto& trapPos : trapPositions_)
			{
				if (AsoUtility::Equals(back->GetTransform().pos,
									   trapPos))
				{
					isTrap = true;
					break;
				}
			}

			if (!isTrap)
			{
				back->Draw();
			}
		}
	}

	// 透明オブジェクト描画
	for (auto& backBlank : placeBackBlankList_)
	{
		backBlank->Draw();
	}
	

	for (auto& placeList : placeFrontList_)
	{
		for (auto& place : placeList)
		{
			place->Draw();
		}
	}
}

void StageBase::DrawPre(void)
{
	/* 背景オブジェクト */
	for (auto& backGround : backGroundList_)
	{
		backGround->Draw();
	}

	// 設置オブジェクト描画
	for (auto& placeBackList : placeBackList_)
	{
		for (auto& back : placeBackList)
		{
			bool isTrap = false;

			for (auto& trapPos : trapPositions_)
			{
				if (AsoUtility::Equals(back->GetTransform().pos,
					trapPos))
				{
					isTrap = true;
					break;
				}
			}

			if (isTrap)
			{
				back->Draw();
			}
		}
	}
}
void StageBase::Release(void)
{
	for (auto& placeList : placeFrontList_)
	{
		for (auto& place : placeList)
		{
			place->Release();
			delete place;
		}
	}

	if (!placeBackList_.empty())
	{
		for (auto& backObjList : placeBackList_)
		{
			for (auto& back : backObjList)
			{
				back->Release();
				delete back;
			}
		}
	}
	if (!placeBackBlankList_.empty())
	{
		for (auto& backBlank : placeBackBlankList_)
		{
			backBlank->Release();
			delete backBlank;
		}
	}
	
	for (auto& backGround : backGroundList_)
	{
		backGround->Release();
		delete backGround;
	}
}



void StageBase::AddStageColliders(ActorBase& _actor)
{
	/*　当たり判定全登録　*/
	for (auto& stageObjList : placeFrontList_)
	{
		for (auto& stageObj : stageObjList)
		{
			for (auto& [mapNum, collision] : stageObj->GetOwnColliders())
			{
				_actor.AddHitCollider(collision);
			}
		}
	}


	if (placeBackList_.empty()) { return; }

	for (auto& backObjList : placeBackList_)
	{
		for (auto& backObj : backObjList)
		{
			for (auto& [mapNum, collision] : backObj->GetOwnColliders())
			{
				_actor.AddHitCollider(collision);
			}
		}
	}
}



void StageBase::SetBlockTypeList(int _mapType, int _xMax, int _yMax)
{
	// マップ数を超えたとき、(最大数-1)に補正する
	int mapType = ((_mapType >= mapNumMax_) ? (mapNumMax_ - 1) : _mapType);
	int mapNum = -1;
	
	placeFrontList_.clear();

	for (int y = 0; y < _yMax; y++)
	{
		std::vector<StageObjBase*> list;

		for (int x = 0; x < _xMax; x++)
		{
			mapNum = -1;

			if (curStageType_ == TYPE::MOVE ||
				curStageType_ == TYPE::MOVE3D)
			{
				mapNum = csvMng_.GetStageMoveNum(mapType, x, y);
			}
			else if (curStageType_ == TYPE::GRAVITY ||
					 curStageType_ == TYPE::GRAVITY3D)
			{
				mapNum = csvMng_.GetStageGravityNum(mapType, x, y);
			}

			// ステージ情報割り当て
			StageObjBase* param = SetParam(mapNum, x, y);
			if (param == nullptr) { continue; }

			// 行配置リストに格納
			list.emplace_back(param);
		}

		// ステージ配置リストに格納
		placeFrontList_.emplace_back(list);
	}
}

void StageBase::SetBlockBackTypeList(int _mapType, int _xMax, int _yMax)
{
	// マップ数を超えたとき、(最大数-1)に補正する
	int mapjType = ((_mapType >= mapNumMax_) ? (mapNumMax_ - 1) : _mapType);
	int mapNum = -1;

	placeBackList_.clear();

	for (int y = 0; y < _yMax; y++)
	{
		std::vector<StageObjBase*> list;

		for (int x = 0; x < _xMax; x++)
		{
			mapNum = -1;

			if (curStageType_ == TYPE::MOVE3D)
			{
				mapNum = csvMng_.GetStageBackMoveNum(mapjType, x, y);
			}
			else if (curStageType_ == TYPE::GRAVITY3D)
			{
				mapNum = csvMng_.GetStageBackGravityNum(mapjType, x, y);
			}

			// ステージ情報割り当て
			StageObjBase* param = SetParamBack(mapNum, x, y);
			if (param != nullptr)
			{
				// 行配置リストに格納
				list.emplace_back(param);
			}

			else
			{
				param = SetParamBack(static_cast<int>(BLOCK_TYPE::WALL), x, y, BACK_ALPHA, false);
				placeBackBlankList_.emplace_back(param);
			}
		}

		// ステージ配置リストに格納
		placeBackList_.emplace_back(list);
	}
}

void StageBase::SetBackGroundList(int _xMax, int _yMax)
{
	/* 背景オブジェクト割り当て */
	const float BACK_ALPHA = 1.0f;
	float posZ = ((static_cast<int>(curStageType_) % 2 == 0) ?
					((STAGE_POS.z + BLOCK_OFFSET.z) * BLOCK_SCALE) :
					((STAGE_POS.z + (BLOCK_OFFSET.z * 2)) * BLOCK_SCALE)
				  );

	for (int y = 0; y < _yMax; y++)
	{
		std::vector<StageObjBase*> list;

		for (int x = 0; x < _xMax; x++)
		{
			// ステージ情報割り当て
			StageObjWall* param = new StageObjWall(x, y, static_cast<int>(BLOCK_TYPE::WALL), BACK_ALPHA, false);

			param->Init(VGet((x * (BLOCK_OFFSET.x * BLOCK_SCALE) + STAGE_POS.x),
							 (y * (BLOCK_OFFSET.y * BLOCK_SCALE) + STAGE_POS.y),
							 posZ));

			// 行配置リストに格納
			backGroundList_.emplace_back(param);
		}
	}
}


void StageBase::ChangeFrontObjects(void)
{
	/* 前が壁、後ろが壁以外の場合、前の壁を透過 */
	for (auto& frontPlace : placeFrontList_)
	{
		for (auto& front : frontPlace)
		{
			// 前ステージが壁以外はスキップ
			if (front->GetObjType() != static_cast<int>(BLOCK_TYPE::WALL)) { continue; }

			// 後ろに壁がないor前が壁以外の場合、前を透過
			if (!_IsHasBackWallAt(front->GetObjArrayPos()))
			{
				front->SetAlpha(FRONT_ALPHA);
			}
		}
	}
}
bool StageBase::_IsHasBackWallAt(const Vector2& _pos) const
{
	for (const auto& backPlace : placeBackList_)
	{
		for (const auto& back : backPlace)
		{
			// 座標が前後で一致しない場合はスキップ
			if (_pos.x != back->GetObjArrayPos().x
				|| _pos.y != back->GetObjArrayPos().y)
			{ continue; }

			// 後ろが壁であれば true
			if (back->GetObjType() == static_cast<int>(BLOCK_TYPE::WALL))
			{
				return true;
			}
		}
	}
	return false;
}