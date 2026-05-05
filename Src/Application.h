#pragma once
#include <string>
#include <memory>
class FpsController;

class Application
{

public:

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = (16 * 100);
	static constexpr int SCREEN_SIZE_Y = (9	* 100);
	static constexpr int SCREEN_HALF_X = (SCREEN_SIZE_X / 2);
	static constexpr int SCREEN_HALF_Y = (SCREEN_SIZE_Y / 2);


	// 重力(メートルの値をセンチメートルに変える)
	static constexpr float GRAVITY = (9.81f * 100.0f);
	static constexpr float GRAVITY_SCALE = 0.7f;


	static void CreateInstance(void);
	static Application& GetInstance(void) { return *instance_;};
	void DestroyInstance(void);


	void Initialize(void);

	/// @brief 実行処理
	void Run(void);


	/// @brief 初期化成功しているか否か
	bool IsInitFail(void) const { return isInitFail_; };

	/// @brief メモリ解放成功しているか否か
	bool IsReleaseFail(void) const { return isReleaseFail_; };

	/// @brief 重力の取得
	float GetGravityPow(void) const { return (GRAVITY * GRAVITY_SCALE); }

	/// @brief ゲーム終了処理
	void SetGameEnd(void) { isGame_ = false; };
	

private:	

	// 静的インスタンス
	static Application* instance_;

	// FPS制御
	std::unique_ptr<FpsController> fpsController_;

	// ゲームを継続するか否か
	bool isGame_;

	// 初期化失敗してるか否か
	bool isInitFail_;

	// 解放失敗しているか否か
	bool isReleaseFail_;


	Application(void);
	~Application(void) = default;
	
	/* コピーコンストラクタ対策 */
	Application(const Application&)			   = delete;
	Application& operator=(const Application&) = delete;
	Application(Application&&)			  = delete;
	Application& operator=(Application&&) = delete;

	/// @brief エフェクシアの初期化処理
	void InitEffekseer(void);
};