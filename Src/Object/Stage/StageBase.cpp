#include "StageBase.h"
#include <cassert>
#include "../Actor/ActorBase.h"
#include "../../CSV/CsvManager.h"
#include "../StageObj/StageObjBase.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Common/Vector2.h"
#include "../../Utility/AsoUtility.h"
#include "../Common/Transform.h"


StageBase::StageBase(TYPE stageType, int mapNum) :
	resMng_(ResourceManager::GetInstance()),
	sceneMng_(SceneManager::GetInstance()),
	stageType_(stageType),
	mapNumMax_(mapNum)
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
	InitList();
}

void StageBase::Update(void)
{
	for (auto placeList : placeType_)
	{
		for (auto& place : placeList)
		{
			place->Update();
		}
	}
}

void StageBase::Draw(void)
{
	// 設置オブジェクト描画
	for (auto placeList : placeType_)
	{
		for (auto& place : placeList)
		{
			place->Draw();
		}
	}
}
void StageBase::Release(void)
{
	for (auto placeList : placeType_)
	{
		for (auto place : placeList)
		{
			place->Release();
			delete place;
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

			if (stageType_ == TYPE::MOVE)
			{
				mapNum = CsvManager::GetInstance().GetStageMoveNum(type, x, y);
			}
			else if (stageType_ == TYPE::GRAVITY)
			{
				mapNum = CsvManager::GetInstance().GetStageGravityNum(type, x, y);
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