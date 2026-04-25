#include "Application.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <cassert>
#include "./Manager/InputManager.h"
#include "./Manager/ResourceManager.h"
#include "./Manager/SceneManager.h"
#include "./Manager/SoundManager.h"
#include "./CSV/CsvManager.h"
#include "./Common/FpsController.h"

// シングルトンインスタンス
Application* Application::instance_ = nullptr;


void Application::CreateInstance(void)
{
	/* インスタンス生成処理 */

	// インスタンス未生成時 生成
	if (instance_ == nullptr) { instance_ = new Application(); };

	instance_->Init(); // 初期化処理
}

Application::Application(void)
	: isGame_(true)
	, isInitFail_(false)
	, isReleaseFail_(false)
{
}

void Application::Init(void)
{
	/* 初期化処理 */

	// ゲーム名
	SetWindowText(GAME_NAME);

	// ウィンドウサイズ
	const int COLOR_BIT = 32;
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, COLOR_BIT);

	ChangeWindowMode(IS_WINDOW);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		OutputDebugString("\nDxLibが初期化されていません");
		assert(false);
	}

	// Effekseerの初期化
	InitEffekseer();

	// 乱数のシード値を設定する
	DATEDATA date;

	// 現在時刻を取得する
	GetDateTime(&date);

	// 乱数の初期値を設定する
	// 設定する数値によって、ランダムの出方が変わる
	SRand(date.Year + date.Mon + date.Day + date.Hour + date.Min + date.Sec);

	// 入力制御初期化
	SetUseDirectInputFlag(true);


	// FPS制御初期化
	fpsController_ = std::make_unique<FpsController>(FRAME_RATE);


	/* 各マネージャインスタンスの明示的生成 */
	InputManager::CreateInstance();
	CsvManager::CreateInstance();
	ResourceManager::CreateInstance();
	SoundManager::CreateInstance();
	SceneManager::CreateInstance();
}

void Application::Run(void)
{
	/*　実行処理　*/

	// ゲームループ
	while (ProcessMessage() == 0 && isGame_)
	{
		// 更新処理
		InputManager::GetInstance().Update();
		SceneManager::GetInstance().Update();

		// 描画処理
		SceneManager::GetInstance().Draw();

		if (SceneManager::GetInstance().GetIsDebugMode())
		{
			// 平均FPS描画
			fpsController_->Draw();
		}

		ScreenFlip();

		// 理想FPS経過待ち
		fpsController_->Wait();
	}
}

void Application::Destroy(void)
{
	/*　インスタンス削除処理　*/

	/* 各マネージャインスタンスのメモリ解放処理 */
	SceneManager::GetInstance().Destroy();
	SoundManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	CsvManager::GetInstance().Destroy();
	InputManager::GetInstance().Destroy();


	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	// インスタンスのメモリ解放
	delete instance_;
}

void Application::InitEffekseer(void)
{
	/* Effekseerの初期化 */
	if (Effekseer_Init(EFFECT_VIRW_MAX) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
