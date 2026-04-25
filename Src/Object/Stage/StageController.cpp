#include "StageController.h"
#include "../Actor/ActorBase.h"
#include "./StageBase.h"
#include "./StageMove.h"
#include "./StageGravity.h"

StageController::StageController(bool _isMoveStage)
	: isMoveStage_(_isMoveStage)
	, curStageType_(STAGE_TYPE::NONE), stage_(nullptr)
	, isStageClear_(false)
	, curStageNum_(0)
	, STAGE_SIZE_MAX(((_isMoveStage == true)
						? CsvManager::GetInstance().GetStageMoveMapNum()
						: CsvManager::GetInstance().GetStageGravityMapNum()))
{
	
}


void StageController::Init(void)
{
	curStageNum_ = 0;

	clearStageNums_.clear();

	// ステージを生成
	SetStageType((isMoveStage_ == true)
					? STAGE_MOVE_TYPE[curStageNum_]
					: STAGE_GRAVITY_TYPE[curStageNum_]);
}

void StageController::Update(void)
{
	stage_->Update();
}

void StageController::Draw(void)
{
	stage_->Draw();
}

void StageController::DrawPre(void)
{
	stage_->DrawPre();
}

void StageController::DrawDebug(void)
{
	/*
#ifdef _DEBUG
	std::string text = "";
	switch (stageType_)
	{
		case STAGE_TYPE::MOVE: { text = "MOVE"; } break;
		case STAGE_TYPE::MOVE3D: { text = "MOVE3D"; } break;
		case STAGE_TYPE::GRAVITY: { text = "GRAVITY"; } break;
		case STAGE_TYPE::GRAVITY3D: { text = "GRAVITY3D"; } break;

		case STAGE_TYPE::CLEAR: { text = "STAGE_CLEAR"; } break;
		default: { text = "Stage:None"; } break;
	}

	DrawString(0, 32, text.c_str(), 0xff0000);
#endif*/
}

void StageController::Release(void)
{
	stage_->Release();
	delete stage_;
}


void StageController::SetStageType(StageController::STAGE_TYPE _type)
{
	curStageType_ = _type;

	if (stage_ != nullptr)
	{
		stage_->Release();
	}

	// ステージ登録
	if (_type == STAGE_TYPE::MOVE ||
		_type == STAGE_TYPE::MOVE3D)
	{
		// 移動ステージ(MOVE3D時、奥行追加)
		stage_ = new StageMove((_type == STAGE_TYPE::MOVE3D));
	}
	else if (_type == STAGE_TYPE::GRAVITY ||
			 _type == STAGE_TYPE::GRAVITY3D)
	{
		// 重力ステージ(GRAVITY3D時、奥行追加)
		stage_ = new StageGravity((_type == STAGE_TYPE::GRAVITY3D));
	}

	// ステージ初期化
	stage_->Init(GetStageChoice());
}

int StageController::GetStageChoice(void)
{
	// 抽選リストがない場合、ステージ０番目を指定
	int retStageNum = 0;

	// 抽選リスト
	std::vector<int> choiceList;
	choiceList.clear();

	for (int i = 0; i < STAGE_SIZE_MAX; i++)
	{
		bool isChoice = true;

		for (int& exclusion : clearStageNums_)
		{
			// 除外リストにある場合、抽選リストから除外
			if (i == exclusion)
			{
				isChoice = false;
				break;
			}
		}

		// 抽選リストに格納
		if (isChoice)
		{
			choiceList.emplace_back(i);
		}
	}

	if (choiceList.size() != 0)
	{
		// 抽選リストからステージを指定
		int rand = GetRand(static_cast<int>(choiceList.size()) - 1);
		retStageNum = choiceList[rand];
	}

	choiceList.clear();

	return retStageNum;
}

const VECTOR& StageController::GetGoalPos(int _num)
{
	return ((_num == 0)
			? stage_->GetGoalPos() :
			  stage_->GetGoalBackPos());
}

void StageController::ChangeStages(void)
{
	// クリアしたステージ番号を格納
	clearStageNums_.emplace_back(stage_->GetStageNum());
	
	// 選択したステージの進行する最大回数
	const size_t CHOICE_STAGE_MAX = ((isMoveStage_) ? STAGE_MOVE_TYPE.size() : STAGE_GRAVITY_TYPE.size());

	if (++curStageNum_ < CHOICE_STAGE_MAX)
	{
		// ステージが全て進行していない時、ステージを再生成
		SetStageType((isMoveStage_ == true)
						? STAGE_MOVE_TYPE[curStageNum_]
						: STAGE_GRAVITY_TYPE[curStageNum_]);
	}
	else
	{
		isStageClear_ = true;
	}
}
