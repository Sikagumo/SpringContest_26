#include "TitleScene.h"
#include <DxLib.h>
#include <functional>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager.h"
#include "../Common/Vector2.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/SkyDome/SkyDome.h"


TitleScene::TitleScene(void)
	:SceneBase(),
	isSelected_(false),
	state_(TITLE_STATE::SELECT_START)
{
}


void TitleScene::Init(void)
{
    selectUI_[static_cast<int>(TITLE_STATE::SELECT_MOVE) - 2].pos    = new Vector2(0, Application::SCREEN_HALF_Y);
    selectUI_[static_cast<int>(TITLE_STATE::SELECT_GRAVITY) - 2].pos = new Vector2(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y);
    selectUI_[static_cast<int>(TITLE_STATE::SELECT_CANCEL) - 2].pos  = new Vector2(Application::SCREEN_SIZE_X, Application::SCREEN_HALF_Y);

    selectUI_[0].color = 0;
    selectUI_[1].color = 0;
    selectUI_[2].color = 0;

    ChangeTitleState(TITLE_STATE::SELECT_START);
}

void TitleScene::Update(void)
{
	// シーン遷移
	if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
	}

    // タイトル状態処理
    updateStateFunc_();
}

void TitleScene::Draw(void)
{
 	DrawString(Application::SCREEN_HALF_X-100, Application::SCREEN_HALF_Y, 
			   "Spaceでゲームスタート", 0x0);

    const Vector2 UI_SIZE = Vector2(300, 500);
    DrawBox(selectUI_[0].pos->x, (selectUI_[0].pos->y - (UI_SIZE.y / 2)),
            (selectUI_[0].pos->x + UI_SIZE.x), selectUI_[0].pos->y + (UI_SIZE.y / 2), selectUI_[0].color, true);
}

void TitleScene::Release(void)
{
}


void TitleScene::ChangeTitleState(TITLE_STATE _state)
{
    if (_state == TITLE_STATE::SELECT_START)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectStart, this);
    }
    if (_state == TITLE_STATE::GAME_END)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_GameEnd, this);
    }
    if (_state == TITLE_STATE::SELECT_MOVE)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectMove, this);
    }
    if (_state == TITLE_STATE::SELECT_GRAVITY)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectGravity, this);
    }
    if (_state == TITLE_STATE::SELECT_CANCEL)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectCancel, this);
    }
}

void TitleScene::Update_SelectStart(void)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
    {
        ChangeTitleState(TITLE_STATE::SELECT_MOVE);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::GAME_END, TITLE_STATE::GAME_END);

}
void TitleScene::Update_GameEnd(void)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
    {
        // ゲーム終了
        Application::GetInstance().SetGameEnd();
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::SELECT_START, TITLE_STATE::SELECT_START);

}
void TitleScene::Update_SelectMove(void)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
    {
        sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::SELECT_CANCEL, TITLE_STATE::SELECT_GRAVITY);

}
void TitleScene::Update_SelectGravity(void)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
    {
        sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::SELECT_MOVE, TITLE_STATE::SELECT_CANCEL);
}

void TitleScene::Update_SelectCancel(void)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
    {
        ChangeTitleState(TITLE_STATE::SELECT_START);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::SELECT_GRAVITY, TITLE_STATE::SELECT_MOVE);

}
void TitleScene::ChangeStateProc(TITLE_STATE _selectUp, TITLE_STATE _selectDown)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_UP)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_LEFT))
    {
        ChangeTitleState(_selectUp);
    }

    //
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DOWN)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_RIGHT))
    {
        ChangeTitleState(_selectDown);
    }
}
