#include "StageBase.h"
#include "../../CSV/CsvManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Common/Vector2.h"
#include "../Common/Transform.h"


StageBase::StageBase(void):
	sceneMng_(SceneManager::GetInstance()),
	resMng_(ResourceManager::GetInstance())
{
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
			place->viewParam->Update();
		}
	}
}

void StageBase::Draw(void)
{
	for (auto placeList : placeType_)
	{
		for (auto& place : placeList)
		{
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

			BlockParam* param = new BlockParam();
			SetParam(*param, type, x, y);

			// 行配置リストに格納
			list.emplace_back(param);
		}

		// ステージ配置リストに格納
		placeType_.emplace_back(list);
	}
}