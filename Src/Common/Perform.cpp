#include "Perform.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"


Perform::Perform(void)
{
    Init();
}

void Perform::Init(void)
{
    type_ = TYPE::NONE;
    timeStop_ = timeSlow_ = timeShake_ = 0.0f;
    termShake_ = termSlow_ = 0;
    isStop_ = false;
    screen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);
    shakePos_ = {};
}

void Perform::Update(void)
{
    // タイマーが全て終了時、処理終了
    if (timeSlow_ <= 0.0f && timeStop_ <= 0.0f) { return; }

    // 時間減少処理
    TimeDecrement();

    // 各演出処理
    Performance();
}

void Perform::BeforeDraw(void)
{
    if (timeShake_ > 0.0f)
    {
        SetDrawScreen(screen_);
    }
}

void Perform::Draw(void)
{
    if (timeShake_ > 0.0f)
    {
        DrawGraph(shakePos_.x, shakePos_.y, screen_, true);
    }
}


void Perform::SetHitStop(float _time)
{
    if (type_ != TYPE::NONE) { return; }
    type_ = TYPE::HIT_STOP;
    timeStop_ = _time;
}

/*
void Perform::SetHitSlow(float _slowTime, float _stopTime)
{
    if (type_ != TYPE::NONE) { return; }

    type_ = TYPE::HIT_SLOW;
    timeSlow_ = _slowTime;
    timeStop_ = _stopTime;
}
*/

void Perform::TimeDecrement(void)
{
    float delta = SceneManager::GetInstance().GetDeltaTime();

    // スロータイマーが0以降で減少
    timeSlow_ -= ((timeSlow_ > 0.0f) ?
        delta : 0.0f);
    // 停止タイマーが0以降、スロータイマー終了時に減少                
    timeStop_ -= ((timeStop_ > 0.0f && timeSlow_ <= 0.0f) ?
        delta : 0.0f);
}

void Perform::Performance(void)
{
    bool ret = false;

    switch (type_)
    {
        case TYPE::HIT_STOP:
            ret = ProcHitStop();
        break;

        case TYPE::HIT_SLOW:
            ret = ProcHitSlow();
        break;

        case TYPE::SLOW:
            ret = ProcSlow();
        break;

        case TYPE::SHAKE:
            //ProcShake();
        break;

        case TYPE::SLOW_SHAKE:
            //ret = ProcShakeSlow();
        break;

        default:break;
    }
    isStop_ = ret;
}

bool Perform::ProcHitStop(void)
{
    /*　ヒットストップ演出　*/
    bool ret = false;
    float delta = SceneManager::GetInstance().GetDeltaTime();

    // 停止タイマーが0より大きい時、減少                
    timeStop_ -= ((timeStop_ > 0.0f) ? delta : 0.0f);

    // 停止終了時、無効状態化
    type_ = ((timeStop_ <= 0.0f) ? TYPE::NONE : type_);

    // 停止タイマーが0より大きい時、停止
    return (timeStop_ > 0.0f);
}

bool Perform::ProcHitSlow(void)
{
    /* ヒットスロー演出 */

    bool isStop = false;

    // 遅延処理
    bool isSlow = ProcSlow();


    if (timeSlow_ <= 0.0f)
    {
        // 遅延終了時、停止処理有効
        isStop_ = ProcHitStop();
    }

    // 停止中・遅延中は、true
    return (isSlow || isStop);
}

bool Perform::ProcSlow(void)
{
    /*　スロー演出　*/
    float delta = SceneManager::GetInstance().GetDeltaTime();
    int slowCnt = static_cast<int>(timeSlow_ * 10.0f);

    // 遅延タイマーが0より大きい時、減少
    timeSlow_ -= ((timeSlow_ > 0.0f) ? delta : 0.0f);

    // 遅延終了時、無効状態化
    type_ = ((timeSlow_ > 0.0f) ? TYPE::NONE : type_);

    // 遅延タイマーが有効・一定の間隔で、true
    return (timeSlow_ > 0.0f && (slowCnt % termSlow_) == 0);
}

void Perform::ProcShakeSmooth(void)
{
    
}

void Perform::ProcShake(void)
{
    
}