#include "StageBase.h"
#include <cassert>
#include "../Actor/ActorBase.h"
#include "../../CSV/CsvManager.h"
#include "../StageObj/StageObjBase.h"
#include "../StageObj/StageObjWall.h"
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
	mapNumMax_(mapNum), mapBackNumMax_(mapBackNum),
	playersPos_{ AsoUtility::VECTOR_ZERO , AsoUtility::VECTOR_ZERO },
	goalPos_(AsoUtility::VECTOR_ZERO), goalPosBack_(AsoUtility::VECTOR_ZERO)
{
}

void StageBase::Init(int _curStageNum)
{
	curStageNum_ = GetRand(mapNumMax_ - 1);

	for (VECTOR& pos : playersPos_)
	{
		pos = AsoUtility::VECTOR_ZERO;
	}
	goalPos_ = goalPosBack_ = AsoUtility::VECTOR_ZERO;

	StageChoice(curStageNum_);
	
}
void StageBase::StageChoice(int _stageNum)
{
	int num = _stageNum;

	// リスト外の場合、０番を読み込む
	if (num >= (mapNumMax_ - 1) || num < 0) { curStageNum_ = num = 0; }

	// ステージ配置処理
	SetBlockTypeList(num, CsvManager::STAGE_X, CsvManager::STAGE_Y);

	// ステージ奥配置処理
	if (stageType_ == TYPE::MOVE3D ||
		stageType_ == TYPE::GRAVITY3D)
	{
		SetBlockBackTypeList(num, CsvManager::STAGE_X, CsvManager::STAGE_Y);

		// 前ステージの透過
		ChangeFrontObjects(num);
	}

	SetBlockBackList(CsvManager::STAGE_X, CsvManager::STAGE_Y);
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

void StageBase::DrawPre(void)
{
	/* 背景オブジェクト */
	for (auto& backGround : backGroundList_)
	{
		for (auto& bg : backGround)
		{
			bg->Draw();
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

	if (!placeBackType_.empty())
	{
		for (auto& backObjList : placeBackType_)
		{
			for (auto& back : backObjList)
			{
				back->Release();
				delete back;
			}
		}
	}
	
	for (auto& backGround : backGroundList_)
	{
		for (auto& back : backGround)
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
	
	placeType_.clear();

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

	placeBackType_.clear();

	for (int y = 0; y < _yMax; y++)
	{
		std::vector<StageObjBase*> list;

		for (int x = 0; x < _xMax; x++)
		{
			mapNum = -1;

			if (stageType_ == TYPE::MOVE3D)
			{
				mapNum = csvMng_.GetStageBackMoveNum(type, x, y);
			}
			else if (stageType_ == TYPE::GRAVITY3D)
			{
				mapNum = csvMng_.GetStageBackGravityNum(type, x, y);
			}

			// ステージ情報割り当て
			StageObjBase* param = SetParamBack(mapNum, x, y);
			if (param == nullptr) { continue; }

			// 行配置リストに格納
			list.emplace_back(param);
		}

		// ステージ配置リストに格納
		placeBackType_.emplace_back(list);
	}
}

void StageBase::SetBlockBackList(int _xMax, int _yMax)
{
	/* 背景オブジェクト割り当て */
	const float BACK_ALPHA = 1.0f;
	float posZ = ((static_cast<int>(stageType_) % 2 == 0) ?
					((STAGE_POS.z + BLOCK_OFFSET.z) * BLOCK_SCALE) :
					((STAGE_POS.z + (BLOCK_OFFSET.z * 2)) * BLOCK_SCALE)
				  );

	for (int y = 0; y < _yMax; y++)
	{
		std::vector<StageObjBase*> list;

		for (int x = 0; x < _xMax; x++)
		{
			// ステージ情報割り当て
			StageObjWall* param = new StageObjWall(x, y, 0, BACK_ALPHA, false);

			param->Init(VGet((x * (BLOCK_OFFSET.x * BLOCK_SCALE) + STAGE_POS.x),
							 (y * (BLOCK_OFFSET.y * BLOCK_SCALE) + STAGE_POS.y),
							 posZ));
			// 行配置リストに格納
			list.emplace_back(param);
		}

		// ステージ背景配置リストに格納
		backGroundList_.emplace_back(list);
	}
}

void StageBase::ChangeFrontObjects(int _stageType)
{
	// 要素外は処理終了
	if (placeType_.size() <= _stageType) { return; }

	for (auto& front : placeType_[_stageType])
	{
		for (auto& back : placeBackType_[_stageType])
		{
			if (front->GetTransform().pos.x == back->GetTransform().pos.x &&
				front->GetTransform().pos.y == back->GetTransform().pos.y &&
				front->GetObjType() != -1 && back->GetObjType() == -1)
			{
				const float ALPHA_FLONT = 0.1f;
				front->SetAlpha(ALPHA_FLONT);
			}
		}
	}
}
