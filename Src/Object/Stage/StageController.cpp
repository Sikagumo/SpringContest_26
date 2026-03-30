#include "StageController.h"
#include "../Actor/ActorBase.h"
#include "./StageBase.h"
#include "./StageMove.h"
#include "./StageGravity.h"

StageController::StageController(void) :
	stageType_(STAGE_TYPE::NONE), stage_(nullptr),
	isStageClear_(false)
{
}


void StageController::Init(void)
{
	SetStageType(STAGE_TYPE::MOVE);
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
	stage_->DrawDebug();

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
#endif
}

void StageController::Release(void)
{
	stage_->Release();
	delete stage_;
}


void StageController::SetStageType(StageController::STAGE_TYPE _type)
{
	stageType_ = _type;

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

	stage_->Init();
}

const VECTOR& StageController::GetGoalPos(int _num)
{
	return ((_num == 0)
			? stage_->GetGoalPos() :
			  stage_->GetGoalBackPos());
}

void StageController::ChangeStages(void)
{
	if (stageType_ == STAGE_TYPE::CLEAR) { return; }

	int stageType = static_cast<int>(stageType_) + 1;
	stageType_ = static_cast<STAGE_TYPE>(stageType);

	SetStageType(stageType_);
}
