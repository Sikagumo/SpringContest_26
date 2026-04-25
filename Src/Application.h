#pragma once
#include <string>
#include <memory>
class FpsController;

class Application
{

public:

	// ゲーム名
	const char* GAME_NAME = "ちぇんじでGO";
	
	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = (16 * 100);
	static constexpr int SCREEN_SIZE_Y = (9	* 100);
	static constexpr int SCREEN_HALF_X = (SCREEN_SIZE_X / 2);
	static constexpr int SCREEN_HALF_Y = (SCREEN_SIZE_Y / 2);

	// 固定FPS数
	static constexpr int FRAME_RATE = 60;

	// 重力(メートルの値をセンチメートルに変える)
	static constexpr float GRAVITY = (9.81f * 100.0f);
	static constexpr float GRAVITY_SCALE = 0.7f;


	/// @brief 明示的にインスタンス生成処理
	static void CreateInstance(void);

	/// @brief アプリケーションマネージャ取得処理
	static Application& GetInstance(void) { return *instance_;};

	/// @brief 初期化 
	void Init(void);

	/// @brief 実行処理
	void Run(void);

	/// @brief インスタンス削除処理
	void Destroy(void);


	/// @brief 初期化成功しているか否か
	bool IsInitFail(void) const { return isInitFail_; };

	/// @brief メモリ解放成功しているか否か
	bool IsReleaseFail(void) const { return isReleaseFail_; };

	/// @brief 重力の取得
	float GetGravityPow(void) const { return (GRAVITY * GRAVITY_SCALE); }

	/// @brief ゲーム終了処理
	void SetGameEnd(void) { isGame_ = false; };
	

private:

	// ゲームをウィンドウで起動するか否か
	static constexpr bool IS_WINDOW = true;

	// エフェクト表示最大数
	static constexpr int EFFECT_VIRW_MAX = 8000;
	

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


	/// @brief デフォルトコンストラクタ
	Application(void);

	/// @brief デフォルトデストラクタ
	~Application(void) = default;
	
	/* コピーコンストラクタ対策 */
	Application(const Application&)			   = delete;
	Application& operator=(const Application&) = delete;
	Application(Application&&)			  = delete;
	Application& operator=(Application&&) = delete;

	/// @brief エフェクシアの初期化処理
	void InitEffekseer(void);
};