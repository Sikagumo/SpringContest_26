#include "StageBase.h"
#include <cassert>
#include "../Actor/ActorBase.h"
#include "../../CSV/CsvManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Common/Vector2.h"
#include "../../Utility/AsoUtility.h"
#include "../Common/Transform.h"


StageBase::StageBase(void) :
	resMng_(ResourceManager::GetInstance()),
	sceneMng_(SceneManager::GetInstance())
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
			if (place->viewParam == nullptr) { continue; }

			place->viewParam->Update();
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
			if (place->viewParam == nullptr) { continue; }

			MV1DrawModel(place->viewParam->modelId);
		}
	}
}
void StageBase::Release(void)
{
	for (auto placeList : placeType_)
	{
		for (auto place : placeList)
		{
			if (place->viewParam == nullptr) { continue; }

			place->viewParam->Release();
			delete place->viewParam;
		}
	}
}

void StageBase::SetBlockTypeList(int _type, int _xMax, int _yMax)
{
	for (int y = 0; y < _yMax; y++)
	{
		std::vector<StageBase::BlockParam*> list;

		for (int x = 0; x < _xMax; x++)
		{
			int type = CsvManager::GetInstance().GetStageMoveNum(_type, x, y);

			// 要素以外の値の時、処理終了
			if (type <= -1) { continue; }

			// ステージ情報割り当て
			BlockParam* param = new BlockParam();
			SetParam(*param, type, x, y);

			// 行配置リストに格納
			list.emplace_back(param);
		}

		// ステージ配置リストに格納
		placeType_.emplace_back(list);
	}
}