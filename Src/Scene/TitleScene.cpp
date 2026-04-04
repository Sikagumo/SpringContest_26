#include "TitleScene.h"
#include <DxLib.h>
#include <functional>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Common/Vector2F.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/SkyDome/SkyDome.h"


TitleScene::TitleScene(void)
	: SceneBase()
    , isSelected_(false)
    , state_(TITLE_STATE::SELECT_START)
    , updateStateFunc_(nullptr)
    , drawFuncTitle_(nullptr), drawFuncSelect_(nullptr)
    , titleImage_(-1), titleBackImage_(-1)
{
    // 画像読み込み
    titleImage_ = resMng_.LoadHandleId(ResourceManager::SRC::IMG_TITLE);
    titleBackImage_ = resMng_.LoadHandleId(ResourceManager::SRC::IMG_TITLE_BACK);

    resMng_.LoadHandleIds(ResourceManager::SRC::IMGS_UI_TITLE, titleUIHandle_);
    resMng_.LoadHandleIds(ResourceManager::SRC::IMGS_UI_SELECT, selectUIHandle_);
}


void TitleScene::Init(void)
{
    ChangeTitleState(TITLE_STATE::SELECT_START);

    backImagesPos_[0] = Vector2F(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y);
    backImagesPos_[1] = Vector2F(Application::SCREEN_HALF_X + (BACK_HALF_X + Application::SCREEN_SIZE_X),
                                 Application::SCREEN_HALF_Y);
    backImagesPos_[2] = Vector2F(Application::SCREEN_HALF_X + ((BACK_HALF_X + Application::SCREEN_SIZE_X) * 2.0f),
                                 Application::SCREEN_HALF_Y);

    sound_.Play(static_cast<int>(ResourceManager::SRC::BGM_TITLE), true);
}

void TitleScene::Update(void)
{
    // タイトル状態処理
    updateStateFunc_();

    const float BACK_SPEED = 750.0f;

    for (auto& pos : backImagesPos_) 
    {
        break;
        pos.x = ((pos.x < Application::SCREEN_HALF_X + ((BACK_HALF_X + Application::SCREEN_SIZE_X) * 2.0f))
                    ? pos.x + (BACK_SPEED * sceneMng_.GetDeltaTime())
                    : Application::SCREEN_HALF_X + ((BACK_HALF_X + Application::SCREEN_SIZE_X) * 2.0f));
    }

    if (input_.IsTrgDown(InputManager::TYPE::SELECT_UP, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DOWN, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_LEFT, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_RIGHT, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)

        || input_.IsTrgDown(InputManager::TYPE::SELECT_UP, Input::JOYPAD_NO::PAD2)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DOWN, Input::JOYPAD_NO::PAD2)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_LEFT, Input::JOYPAD_NO::PAD2)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_RIGHT, Input::JOYPAD_NO::PAD2)
        || input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
    {
        sound_.Play(static_cast<int>(ResourceManager::SRC::SE_SELECT), false, true);
    }
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2))
    {
        sound_.Play(static_cast<int>(ResourceManager::SRC::SE_CLICK), false, true);
    }
}

void TitleScene::Draw(void)
{
    const float BACK_SCALE = 2.0f;
    const float BACK_OFFSET = (2040 * BACK_SCALE);

    for (auto pos : backImagesPos_)
    {
        DrawRotaGraph(static_cast<int>(pos.x), static_cast<int>(pos.y)
            , BACK_SCALE, 0.0f, titleBackImage_, true);
        break;
    }

    // タイトル画像
    const int TITLE_POS_X = (Application::SCREEN_HALF_X - 408);
    const int TITLE_POS_Y = 100;
    DrawGraph(TITLE_POS_X, TITLE_POS_Y, titleImage_, true);

    drawFuncTitle_();

    // 状態別描画処理
    if (state_ != TITLE_STATE::SELECT_START &&
        state_ != TITLE_STATE::GAME_END)
    {
        int alpha = (255 - 225);
        const unsigned int SELECT_BACK_COLOR = UtilityCommon::SetColor(255, 255, 0);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
                SELECT_BACK_COLOR, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


        DrawRotaGraph(Application::SCREEN_HALF_X, 100, 1.0, 0.0
            , uiText_[static_cast<int>(UI_TEXT::STAGE_SELECT)], true);

        // 選択画像描画
        drawFuncSelect_();
    }
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
        drawFuncTitle_ = std::bind(&TitleScene::Draw_SelectStart, this);
    }
    if (_state == TITLE_STATE::GAME_END)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_GameEnd, this);
        drawFuncTitle_ = std::bind(&TitleScene::Draw_GameEnd, this);
    }

    if (_state == TITLE_STATE::SELECT_MOVE)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectMove, this);
        drawFuncSelect_ = std::bind(&TitleScene::Draw_SelectMove, this);
    }
    if (_state == TITLE_STATE::SELECT_GRAVITY)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectGravity, this);
        drawFuncSelect_ = std::bind(&TitleScene::Draw_SelectGravity, this);
    }
    if (_state == TITLE_STATE::SELECT_CANCEL)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectCancel, this);
        drawFuncSelect_ = std::bind(&TitleScene::Draw_SelectCancel, this);
    }


    if (drawFuncTitle_ == nullptr)
    {
        // タイトル描画が未割当時、選択開始描画関数にする
        drawFuncTitle_ = std::bind(&TitleScene::Draw_SelectStart, this);
    }
}

void TitleScene::Update_SelectStart(void)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2))
    {
        ChangeTitleState(TITLE_STATE::SELECT_MOVE);
    }
    if (input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
        ||input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
    {
        ChangeTitleState(TITLE_STATE::GAME_END);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::GAME_END, TITLE_STATE::GAME_END);

}
void TitleScene::Update_GameEnd(void)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2)
        || input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
    {
        // ゲーム終了
        Application::GetInstance().SetGameEnd();
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::SELECT_START, TITLE_STATE::SELECT_START);

}
void TitleScene::Update_SelectMove(void)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2))
    {
        // 移動ステージを選択してシーン遷移
        sceneMng_.SetIsStageMove(true);
        sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
    }
    if (input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
    {
        ChangeTitleState(TITLE_STATE::SELECT_START);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::SELECT_CANCEL, TITLE_STATE::SELECT_GRAVITY);

}
void TitleScene::Update_SelectGravity(void)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2))
    {
        // 重力ステージを選択してシーン遷移
        sceneMng_.SetIsStageMove(false);
        sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
    }
    if (input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
    {
        ChangeTitleState(TITLE_STATE::SELECT_START);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::SELECT_MOVE, TITLE_STATE::SELECT_CANCEL);
}

void TitleScene::Update_SelectCancel(void)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2))
    {
        ChangeTitleState(TITLE_STATE::SELECT_START);
    }
    if (input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
    {
        ChangeTitleState(TITLE_STATE::SELECT_START);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::SELECT_GRAVITY, TITLE_STATE::SELECT_MOVE);

}

void TitleScene::Draw_SelectStart(void)
{
    const int TITLE_UI_POS_Y = (Application::SCREEN_HALF_Y + 250);

    int x = 0;
    
    x = (Application::SCREEN_HALF_X - TITLE_UI_OFFSET);
    DrawRotaGraph(x, TITLE_UI_POS_Y
                  , TITLE_UI_SCALE, 0.0
                  , titleUIHandle_[static_cast<int>(TITLE_UI_IMAGE::SELECT_START)], true);


    SetDrawBlendMode(DX_BLENDMODE_SUB, TITLE_NOT_SUB);

    x = (Application::SCREEN_HALF_X + TITLE_UI_OFFSET);
    DrawRotaGraph(x, TITLE_UI_POS_Y
                  , TITLE_NOT_UI_SCALE, 0.0
                  , titleUIHandle_[static_cast<int>(TITLE_UI_IMAGE::GAME_END)], true);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
void TitleScene::Draw_GameEnd(void)
{
    const int TITLE_UI_POS_Y = (Application::SCREEN_HALF_Y + 250);

    int x = 0;

    x = (Application::SCREEN_HALF_X + TITLE_UI_OFFSET);
    DrawRotaGraph(x, TITLE_UI_POS_Y
                  , TITLE_UI_SCALE, 0.0
                  , titleUIHandle_[static_cast<int>(TITLE_UI_IMAGE::GAME_END)], true);


    SetDrawBlendMode(DX_BLENDMODE_SUB, TITLE_NOT_SUB);

    x = (Application::SCREEN_HALF_X - TITLE_UI_OFFSET);
    DrawRotaGraph(x, TITLE_UI_POS_Y
                  , TITLE_NOT_UI_SCALE, 0.0
                  , titleUIHandle_[static_cast<int>(TITLE_UI_IMAGE::SELECT_START)], true);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


void TitleScene::Draw_SelectMove(void)
{
    int x = 0;

    x = Application::SCREEN_HALF_X;
    DrawRotaGraph(x, Application::SCREEN_HALF_Y + TITLE_UI_OFFSET_Y
                  , SELECT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_UI_IMAGE::MOVE_SELECT)], true);


    SetDrawBlendMode(DX_BLENDMODE_ALPHA, SELECT_ALPHA);

    x = (Application::SCREEN_HALF_X - SELECT_UI_OFFSET);
    DrawRotaGraph(x, Application::SCREEN_HALF_Y + TITLE_UI_OFFSET_Y
                  , SELECT_NOT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_UI_IMAGE::BACK)], true);


    x = (Application::SCREEN_HALF_X + SELECT_UI_OFFSET);
    DrawRotaGraph(x, Application::SCREEN_HALF_Y + TITLE_UI_OFFSET_Y
                  , SELECT_NOT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_UI_IMAGE::GRAVITY_NOT_SELECT)], true);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
void TitleScene::Draw_SelectGravity(void)
{
    int x = 0;

    x = (Application::SCREEN_HALF_X + SELECT_UI_OFFSET);
    DrawRotaGraph(x, Application::SCREEN_HALF_Y + TITLE_UI_OFFSET_Y
                  , SELECT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_UI_IMAGE::GRAVITY_SELECT)], true);


    SetDrawBlendMode(DX_BLENDMODE_ALPHA, SELECT_ALPHA);

    x = (Application::SCREEN_HALF_X - SELECT_UI_OFFSET);
    DrawRotaGraph(x, Application::SCREEN_HALF_Y + TITLE_UI_OFFSET_Y
        , SELECT_NOT_UI_SCALE, 0.0
        , selectUIHandle_[static_cast<int>(SELECT_UI_IMAGE::BACK)], true);

    x = Application::SCREEN_HALF_X;
    DrawRotaGraph(x, Application::SCREEN_HALF_Y + TITLE_UI_OFFSET_Y
                  , SELECT_NOT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_UI_IMAGE::MOVE_NOT_SELECT)], true);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
void TitleScene::Draw_SelectCancel(void)
{
    int x = 0;
    x = (Application::SCREEN_HALF_X - SELECT_UI_OFFSET);
    DrawRotaGraph(x, Application::SCREEN_HALF_Y + TITLE_UI_OFFSET_Y
                  , SELECT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_UI_IMAGE::BACK)], true);


    SetDrawBlendMode(DX_BLENDMODE_ALPHA, SELECT_ALPHA);

    x = Application::SCREEN_HALF_X;
    DrawRotaGraph(x, Application::SCREEN_HALF_Y + TITLE_UI_OFFSET_Y
                  , SELECT_NOT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_UI_IMAGE::MOVE_NOT_SELECT)], true);


    x = (Application::SCREEN_HALF_X + SELECT_UI_OFFSET);
    DrawRotaGraph(x, Application::SCREEN_HALF_Y + TITLE_UI_OFFSET_Y
                  , SELECT_NOT_UI_SCALE, 0.0
                  , selectUIHandle_[static_cast<int>(SELECT_UI_IMAGE::GRAVITY_NOT_SELECT)], true);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::ChangeStateProc(TITLE_STATE _selectUp, TITLE_STATE _selectDown)
{
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_UP, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_LEFT, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_UP, Input::JOYPAD_NO::PAD2)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_LEFT, Input::JOYPAD_NO::PAD2))
    {
        ChangeTitleState(_selectUp);
    }

    //
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DOWN, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_RIGHT, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DOWN, Input::JOYPAD_NO::PAD2)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_RIGHT, Input::JOYPAD_NO::PAD2))
    {
        ChangeTitleState(_selectDown);
    }
}
