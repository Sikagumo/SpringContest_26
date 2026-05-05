#pragma once
#include "./Vector2.h"

class Perform
{
public:

    enum class TYPE
    {
        NONE = -1,
        HIT_STOP,
        HIT_SLOW,
        SLOW,
        SHAKE,
        SHAKE_SMOOTH,
        SLOW_SHAKE,
    };

    static constexpr float SHAKE_MAX_X = 8.0f;
    static constexpr float SHAKE_MAX_Y = 5.0f;

    Perform(void);

    ~Perform(void) = default;


    void Initialize(void);

    void Update(void);

    void BeforeDraw(void);

    ///@brief •`‰æˆ—
    void Draw(void);

    /// @brief ‰‰oƒXƒNƒŠ[ƒ“•`‰æ
    //void DrawPerformScreen(void);


    void SetHitStop(float _time);

    //void SetHitSlow(float _slowTime, float _stopTime = 0.0f);

    //void SetSlow(float _time);

    //void SetShake(float _time, int _slowTerm, float _shakeX = SHAKE_MAX_X, float _shakeY = SHAKE_MAX_Y);


    bool GetPerformStop(void) { return isStop_; };


private:

    TYPE type_;
    float timeStop_;
    float timeSlow_;
    float timeShake_;
    float timeShakeMax_;

    // U“®ŠÔŠu(”’l‚ª’á‚¢‚ÆU“®‰ñ”‚ªŒ¸‚é)
    int termShake_;
    int cntShakeX_;
    int cntShakeY_;
    Vector2 shakePos_;

    // ’x‰„ŠÔŠu(”’l‚ª’á‚¢‚Æ‚æ‚è’x‚­‚È‚é)
    int termSlow_;

    bool isStop_;
    int screen_;


    void TimeDecrement(void);

    void Performance(void);

    bool ProcHitStop(void);
    bool ProcHitSlow(void);
    bool ProcSlow(void);
    void ProcShake(void);
    void ProcShakeSmooth(void);
};