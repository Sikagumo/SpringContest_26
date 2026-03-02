#include "StageBase.h"
#include <cassert>
#include "../Actor/ActorBase.h"
#include "../../CSV/CsvManager.h"
#include "../StageObj/StageObjBase.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../CSV/CsvManager.h"
#include "../../Common/Vector2.h"
#include "../../Utility/AsoUtility.h"
#include "../Common/Transform.h"


StageBase::StageBase(TYPE stageType, int mapNum, int mapBackNum) :
	resMng_(ResourceManager::GetInstance()),
	sceneMng_(SceneManager::GetInstance()),
	csvMng_(CsvManager::GetInstance()),
	stageType_(stageType),
	mapNumMax_(mapNum), mapBackNumMax_(mapBackNum)
{
	constexpr VECTOR CONSTRUCT_POS = { -1, -1, -1 };
	for (VECTOR& pos : playersPos_)
	{
		pos = CONSTRUCT_POS;
	}

	goalPos_ = CONSTRUCT_POS;
}

void StageBase::Init(void)
{
	int rand = GetRand(mapNumMax_ - 1);
	SetBlockTypeList(rand, CsvManager::STAGE_MOVE_X, CsvManager::STAGE_MOVE_Y);
	SetBlockBackTypeList(rand, CsvManager::STAGE_MOVE_X, CsvManager::STAGE_MOVE_Y);
}

void StageBase::Update(void)
{
	for (auto& placeList : placeType_)
	{
		for (auto& place : placeList)
		{
			place->Update();
		}
	}

	if (placeBackType_.empty()) { return; }

	for (auto& backList : placeBackType_)
	{
		for (auto& back : backList)
		{
			back->Update();
		}
	}
}

void StageBase::Draw(void)
{
	// 設置オブジェクト描画
	for (auto& placeBackList : placeBackType_)
	{
		for (auto& back : placeBackList)
		{
			back->Draw();
		}
	}
	for (auto& placeList : placeType_)
	{
		for (auto& place : placeList)
		{
			place->Draw();
		}
	}
}
void StageBase::Release(void)
{
	for (auto& placeList : placeType_)
	{
		for (auto& place : placeList)
		{
			place->Release();
			delete place;
		}
	}

	if (placeBackType_.empty()) { return; }

	for (auto& backObjList : placeBackType_)
	{
		for (auto& back : backObjList)
		{
			back->Release();
			delete back;
		}
	}
}

void StageBase::AddStageColliders(ActorBase& _actor)
{
	/*　当たり判定全登録　*/
	for (auto& stageObjList : placeType_)
	{
		for (auto& stageObj : stageObjList)
		{
			for (auto& [mapNum, collision] : stageObj->GetOwnColliders())
			{
				_actor.AddHitCollider(collision);
			}
		}
	}


	if (placeBackType_.empty()) { return; }

	for (auto& backObjList : placeBackType_)
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
	int type = ((_mapType >= mapNumMax_) ? (mapNumMax_ - 1) : _mapType);
	int mapNum = -1;

	for (int y = 0; y < _yMax; y++)
	{
		std::vector<StageObjBase*> list;

		for (int x = 0; x < _xMax; x++)
		{
			mapNum = -1;

			if (stageType_ == TYPE::MOVE ||
				stageType_ == TYPE::MOVE3D)
			{
				mapNum = csvMng_.GetStageMoveNum(type, x, y);
			}
			else if (stageType_ == TYPE::GRAVITY ||
					 stageType_ == TYPE::GRAVITY3D)
			{
				mapNum = csvMng_.GetStageGravityNum(type, x, y);
			}

			// 要素以外の値の時、処理終了
			if (mapNum <= -1) { continue; }

			// ステージ情報割り当て
			StageObjBase* param = SetParam(mapNum, x, y);
			if (param == nullptr) { continue; }

			// 行配置リストに格納
			list.emplace_back(param);
		}

		// ステージ配置リストに格納
		placeType_.emplace_back(list);
	}
}

void StageBase::SetBlockBackTypeList(int _mapType, int _xMax, int _yMax)
{
	// マップ数を超えたとき、(最大数-1)に補正する
	int type = ((_mapType >= mapNumMax_) ? (mapNumMax_ - 1) : _mapType);
	int mapNum = -1;

	for (int y = 0; y < _yMax; y++)
	{
		std::vector<StageObjBase*> list;

		for (int x = 0; x < _xMax; x++)
		{
			mapNum = -1;

			if (stageType_ == TYPE::MOVE ||
				stageType_ == TYPE::MOVE3D)
			{
				mapNum = csvMng_.GetStageBackMoveNum(type, x, y);
			}
			else if (stageType_ == TYPE::GRAVITY ||
				stageType_ == TYPE::GRAVITY3D)
			{
				mapNum = csvMng_.GetStageBackGravityNum(type, x, y);
			}

			// 要素以外の値の時、処理終了
			if (mapNum <= -1) { continue; }

			// ステージ情報割り当て
			StageObjBase* param = SetParamBack(mapNum,
										   static_cast<float>(x),
										   static_cast<float>(y));
			if (param == nullptr) { continue; }

			// 行配置リストに格納
			list.emplace_back(param);
		}

		// ステージ配置リストに格納
		placeBackType_.emplace_back(list);
	}
}
