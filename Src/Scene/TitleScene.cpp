#include "TitleScene.h"
#include <DxLib.h>
#include <functional>
#include "../Application.h"
#include "../Utility/UtilityMath.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Common/Vector2.h"
#include "../Object/SkyDome/SkyDome.h"

// 背景
constexpr float BACK_SCALE = 2.0f;
constexpr float BACK_HALF_X = (2040 / 2) * BACK_SCALE;

// タイトルUIのスケール
constexpr float TITLE_UI_SCALE = (1.0f - 0.0f);
constexpr float TITLE_NOT_UI_SCALE = (1.0f - 0.25f);

// タイトルUIのスケール
constexpr float SELECT_UI_SCALE = (1.0f - 0.15f);
constexpr float SELECT_NOT_UI_SCALE = (1.0f - 0.6f);

// UI位置調整値
constexpr int TITLE_UI_OFFSET = 250;
constexpr int TITLE_UI_OFFSET_Y = 75;
constexpr int SELECT_UI_OFFSET = 500;

constexpr int TITLE_NOT_SUB = (255 - 200);
constexpr int SELECT_ALPHA = (255 - 100);

// タイトルUI縦位置
constexpr int TITLE_UI_POS_Y = (Application::SCREEN_HALF_Y + 250);


TitleScene::TitleScene(void)
	: SceneBase()
    , isSelected_(false)
    , state_(TITLE_STATE::SELECT_START)
    , updateStateFunc_(nullptr)
    , drawFuncTitle_(nullptr), drawFuncSelect_(nullptr)
    , titleImage_(-1), titleBackImage_(-1)
    , selectUIHandle_{}, titleUIHandle_{}
{
    // 画像読み込み
    titleImage_ = resMng_.LoadHandleId(ResourceManager::SRC::IMG_TITLE);
    titleBackImage_ = resMng_.LoadHandleId(ResourceManager::SRC::IMG_TITLE_BACK);

    resMng_.LoadHandleIds(ResourceManager::SRC::IMGS_UI_TITLE, titleUIHandle_);
    resMng_.LoadHandleIds(ResourceManager::SRC::IMGS_UI_SELECT, selectUIHandle_);
}


void TitleScene::Initialize(void)
{
    /* 初期化処理 */
    _ChangeStateProc(TITLE_STATE::SELECT_START);

    sound_.Play(static_cast<int>(ResourceManager::SRC::BGM_TITLE), true);
}

void TitleScene::Update(void)
{
    /* 更新処理 */

    constexpr float BACK_SPEED = 750.0f;

    // タイトル状態処理
    updateStateFunc_();


    /* 効果音再生 */
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
    /* 描画処理 */

    DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y
                      , BACK_SCALE, 0.0f, titleBackImage_, true);

    // タイトル画像
    constexpr int TITLE_POS_X = (Application::SCREEN_HALF_X - 408);
    constexpr int TITLE_POS_Y = 100;
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

void TitleScene::ChangeStateProc(TITLE_STATE _selectUp, TITLE_STATE _selectDown)
{
    /* タイトル状態遷移処理 */

    if (input_.IsTrgDown(InputManager::TYPE::SELECT_UP, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_LEFT, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_UP, Input::JOYPAD_NO::PAD2)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_LEFT, Input::JOYPAD_NO::PAD2))
    {
        _ChangeStateProc(_selectUp);
    }

    //
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DOWN, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_RIGHT, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DOWN, Input::JOYPAD_NO::PAD2)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_RIGHT, Input::JOYPAD_NO::PAD2))
    {
        _ChangeStateProc(_selectDown);
    }
}
void TitleScene::_ChangeStateProc(TITLE_STATE _state)
{
    /* タイトル状態遷移処理の内容 */
    state_ = _state;

    if (state_ == TITLE_STATE::SELECT_START)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectStart, this);
        drawFuncTitle_ = std::bind(&TitleScene::Draw_SelectStart, this);
    }
    else if (state_ == TITLE_STATE::GAME_END)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_GameEnd, this);
        drawFuncTitle_ = std::bind(&TitleScene::Draw_GameEnd, this);
    }
    else if (state_ == TITLE_STATE::SELECT_MOVE)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectMove, this);
        drawFuncSelect_ = std::bind(&TitleScene::Draw_SelectMove, this);
    }
    else if (state_ == TITLE_STATE::SELECT_GRAVITY)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectGravity, this);
        drawFuncSelect_ = std::bind(&TitleScene::Draw_SelectGravity, this);
    }
    else if (state_ == TITLE_STATE::SELECT_CANCEL)
    {
        updateStateFunc_ = std::bind(&TitleScene::Update_SelectCancel, this);
        drawFuncSelect_ = std::bind(&TitleScene::Draw_SelectCancel, this);
    }


    // タイトル描画が未割当時、"選択開始"描画関数にする
    if (drawFuncTitle_ == nullptr)
    {
        drawFuncTitle_ = std::bind(&TitleScene::Draw_SelectStart, this);
    }
}


void TitleScene::Update_SelectStart(void)
{
	/* タイトル状態："選択開始"の更新処理 */

    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2))
    {
        _ChangeStateProc(TITLE_STATE::SELECT_MOVE);
    }
    if (input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
        ||input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
    {
        _ChangeStateProc(TITLE_STATE::GAME_END);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::GAME_END, TITLE_STATE::GAME_END);

}
void TitleScene::Update_GameEnd(void)
{
	/* タイトル状態："ゲーム終了"の更新処理 */
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
	/* タイトル状態："移動ステージ選択"の更新処理 */

    // 移動ステージを選択してシーン遷移
    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2))
    {
        sceneMng_.SetIsStageMove(true);
        sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
    }

    if (input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
    {
        _ChangeStateProc(TITLE_STATE::SELECT_START);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::SELECT_CANCEL, TITLE_STATE::SELECT_GRAVITY);

}
void TitleScene::Update_SelectGravity(void)
{
	/* タイトル状態："重力ステージ選択"の更新処理 */
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
        _ChangeStateProc(TITLE_STATE::SELECT_START);
    }

    // タイトル状態遷移処理
    ChangeStateProc(TITLE_STATE::SELECT_MOVE, TITLE_STATE::SELECT_CANCEL);
}

void TitleScene::Update_SelectCancel(void)
{
	/* タイトル状態："選択キャンセル"の更新処理 */

    if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION, Input::JOYPAD_NO::PAD2))
    {
        _ChangeStateProc(TITLE_STATE::SELECT_START);
    }
    if (input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD1)
        || input_.IsTrgDown(InputManager::TYPE::PAUSE, Input::JOYPAD_NO::PAD2))
    {
        _ChangeStateProc(TITLE_STATE::SELECT_START);
    }

    // タイトル状態遷移入力
    ChangeStateProc(TITLE_STATE::SELECT_GRAVITY, TITLE_STATE::SELECT_MOVE);

}

void TitleScene::Draw_SelectStart(void)
{
	/* タイトル状態："選択開始"の描画処理 */
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
	/* タイトル状態："ゲーム終了"の描画処理 */
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
	/* タイトル状態："移動ステージ選択"の描画処理 */
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
	/* タイトル状態："重力ステージ選択"の描画処理 */
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
	/* タイトル状態："選択キャンセル"の描画処理 */
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