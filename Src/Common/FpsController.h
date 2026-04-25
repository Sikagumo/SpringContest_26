#pragma once
#include <vector>
#include <chrono>
#include <string>
#include <DxLib.h>
#include "../Utility/UtilityCommon.h"

class FpsController
{

public:

    /// @brief コンストラクタ(DxLib_Init前に呼ぶこと)
	/// @param _fixedFps 固定フレームレート(0以下の値は無制限)
    FpsController(int _fixedFps);

    /// @brief デストラクタ
    ~FpsController(void) = default;


    /// @brief 1フレームごとのFPS制御(ScreenFlip後に呼ぶこと）
    void Wait(void);

    /// @brief 画面にFPSを表示
    void Draw(void);

    /// @brief 現在のFPSを取得
    float GetFPS(void) const { return fps_; }


private:

    // 最大FPS
    static constexpr int MAX_FPS = 1200;

	// 平均FPS計算に使用するフレーム数
	static constexpr int AVG_FPS_COUNT = 60;

	// 平均FPSの描画色
	static constexpr UtilityCommon::Color COLOR = UtilityCommon::Color(255, 255, 255);

    // 平均FPSの右上描画位置調整
    static constexpr int MARGIN = 20;

    // 描画フォーマット
    const std::string TEXT_FORMAT = "FPS : %.2f";


    // 指定された固定フレームレート
    const int fixedFps_;

	// 1フレームの理想時間(秒)
	const double idealFrameTime_;

    // 計測用FPS
    float fps_;

    // 平均FPS計測用)(秒単位)
    std::vector<double> timeList_;

    // 前フレームの時間
    std::chrono::high_resolution_clock::time_point prevTime_;

};
