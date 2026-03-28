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
    // 画像読み込み
    resMng_.LoadHandleIds(ResourceManager::SRC::IMGS_UI, selectUIHandle_);
}


void TitleScene::Init(void)
{
    //selectUI_[static_cast<int>(TITLE_STATE::SELECT_MOVE) - 2].pos    = new Vector2(0, Application::SCREEN_HALF_Y);
    //selectUI_[static_cast<int>(TITLE_STATE::SELECT_GRAVITY) - 2].pos = new Vector2(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y);
    //selectUI_[static_cast<int>(TITLE_STATE::SELECT_CANCEL) - 2].pos  = new Vector2(Application::SCREEN_SIZE_X, Application::SCREEN_HALF_Y);

    //selectUI_[0].color = 0;
    //selectUI_[1].color = 0;
    //selectUI_[2].color = 0;

    ChangeTitleState(TITLE_STATE::SELECT_START);
}

void TitleScene::Update(void)
{
	// シーン遷移
	if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		//sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
	}

    // タイトル状態処理
    updateStateFunc_();
}

void TitleScene::Draw(void)
{
 	DrawString(Application::SCREEN_HALF_X-100, Application::SCREEN_HALF_Y, 
			   "Spaceでゲームスタート", 0x0);

    // 状態別描画処理
    if (state_ != TITLE_STATE::SELECT_START &&
        state_ != TITLE_STATE::GAME_END)
    {
        drawStateFunc_();
    }

#ifdef _DEBUG
    std::string text = "";
    text = ((state_ == TITLE_STATE::SELECT_START)   ? "SelectStart"   : text);
    text = ((state_ == TITLE_STATE::GAME_END)       ? "GameEnd"       : text);
    text = ((state_ == TITLE_STATE::SELECT_MOVE)    ? "SelectMove"    : text);
    text = ((state_ == TITLE_STATE::SELECT_GRAVITY) ? "SelectGravity" : text);
    text = ((state_ == TITLE_STATE::SELECT_CANCEL)  ? "SelectCancel"  : text);

    DrawString(0, 8, text.c_str(), 0xff0000);
#endif
}

void TitleScene::Release(void)
{
}


void TitleScene::ChangeTitleState(TITLE_STATE _state)
{
    state_ = _state;

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
        drawStateFunc_ = std::bind(&TitleScene::Draw_SelectMove, this);
    }
    if (_state == TITLE_STATE::SELECT_GRAVITY)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectGravity, this);
        drawStateFunc_ = std::bind(&TitleScene::Draw_SelectGravity, this);
    }
    if (_state == TITLE_STATE::SELECT_CANCEL)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectCancel, this);
        drawStateFunc_ = std::bind(&TitleScene::Draw_SelectCancel, this);
    }
}

void TitleScene::Update_SelectStart(void)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
    {
        ChangeTitleState(TITLE_STATE::SELECT_MOVE);
    }
    if (input_.IsTrgDown(InputManager::TYPE::PAUSE))
    {
        ChangeTitleState(TITLE_STATE::GAME_END);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::GAME_END, TITLE_STATE::GAME_END);

}
void TitleScene::Update_GameEnd(void)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION)
        || input_.IsTrgDown(InputManager::TYPE::PAUSE))
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
    if (input_.IsTrgDown(InputManager::TYPE::PAUSE))
    {
        ChangeTitleState(TITLE_STATE::SELECT_START);
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
    if (input_.IsTrgDown(InputManager::TYPE::PAUSE))
    {
        ChangeTitleState(TITLE_STATE::SELECT_START);
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
    if (input_.IsTrgDown(InputManager::TYPE::PAUSE))
    {
        ChangeTitleState(TITLE_STATE::SELECT_START);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::SELECT_GRAVITY, TITLE_STATE::SELECT_MOVE);

}


void TitleScene::Draw_SelectMove(void)
{
    int x = 0;

    x = Application::SCREEN_HALF_X;
    DrawRotaGraph(x, Application::SCREEN_HALF_Y
                  , SELECT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_IMAGE::MOVE_SELECT)], true);


    SetDrawBlendMode(DX_BLENDMODE_SUB, SELECT_NOT_SUB);

    x = (Application::SCREEN_HALF_X - SELECT_UI_OFFSET);
    DrawRotaGraph(x, Application::SCREEN_HALF_Y
                  , SELECT_NOT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_IMAGE::BACK)], true);

    x = (Application::SCREEN_HALF_X + SELECT_UI_OFFSET);
    DrawRotaGraph(x, Application::SCREEN_HALF_Y
                  , SELECT_NOT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_IMAGE::GRAVITY_NOT_SELECT)], true);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
void TitleScene::Draw_SelectGravity(void)
{
    int x = 0;

    x = (Application::SCREEN_HALF_X + SELECT_UI_OFFSET);
    DrawRotaGraph(x, Application::SCREEN_HALF_Y
                  , SELECT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_IMAGE::GRAVITY_SELECT)], true);


    SetDrawBlendMode(DX_BLENDMODE_SUB, SELECT_NOT_SUB);

    x = (Application::SCREEN_HALF_X - SELECT_UI_OFFSET);
    DrawRotaGraph(x, Application::SCREEN_HALF_Y
        , SELECT_NOT_UI_SCALE, 0.0
        , selectUIHandle_[static_cast<int>(SELECT_IMAGE::BACK)], true);

    x = Application::SCREEN_HALF_X;
    DrawRotaGraph(x, Application::SCREEN_HALF_Y
                  , SELECT_NOT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_IMAGE::MOVE_NOT_SELECT)], true);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
void TitleScene::Draw_SelectCancel(void)
{
    int x = 0;
    x = (Application::SCREEN_HALF_X - SELECT_UI_OFFSET);
    DrawRotaGraph(x, Application::SCREEN_HALF_Y
                  , SELECT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_IMAGE::BACK)], true);


    SetDrawBlendMode(DX_BLENDMODE_SUB, SELECT_NOT_SUB);

    x = Application::SCREEN_HALF_X;
    DrawRotaGraph(x, Application::SCREEN_HALF_Y
                  , SELECT_NOT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_IMAGE::MOVE_NOT_SELECT)], true);

    x = (Application::SCREEN_HALF_X + SELECT_UI_OFFSET);
    DrawRotaGraph(x, Application::SCREEN_HALF_Y
                  , SELECT_NOT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_IMAGE::GRAVITY_NOT_SELECT)], true);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
