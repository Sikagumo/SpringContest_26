#include "Application.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <cassert>
#include "./Manager/InputManager.h"
#include "./Manager/ResourceManager.h"
#include "./Manager/SceneManager.h"
#include "./CSV/CsvManager.h"
#include "./Common/FpsController.h"

// シングルトンインスタンス
Application* Application::instance_ = nullptr;


void Application::CreateInstance(void)
{
	/* インスタンス生成処理 */

	// インスタンス未生成時 生成
	if (instance_ == nullptr) instance_ = new Application();

	instance_->Init(); // 初期化処理
}

Application::Application(void)
	:
	isGame_(true),
	isInitFail_(false),
	isReleaseFail_(false),
	fpsController_(nullptr)
{
}

void Application::Init(void)
{
	/* 初期化処理 */

	// ゲーム名
	SetWindowText(GAME_NAME);


	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// FPS制御初期化
	fpsController_ = new FpsController(FRAME_RATE);

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
	InputManager::CreateInstance();

	CsvManager::CreateInstance();

	// リソース管理初期化
	ResourceManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

}

void Application::Run(void)
{
	/*　実行処理　*/

	// シーン管理マネージャ
	SceneManager& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && isGame_)
	{

		InputManager::GetInstance().Update();

		sceneManager.Update();

		sceneManager.Draw();

		// ゲーム終了
		if (InputManager::GetInstance().IsNew(InputManager::TYPE::PAUSE))
		{
			isGame_ = false;
		}

		if (sceneManager.GetIsDebugMode())
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

	// FPS制御メモリ解放
	delete fpsController_;

	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	
	// シーン管理解放
	SceneManager::GetInstance().Destroy();

	// CSV管理解放
	CsvManager::GetInstance().Destroy();

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

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}



void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
