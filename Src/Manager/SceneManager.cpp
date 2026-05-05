#include "SceneManager.h"
#include <EffekseerForDXLib.h>
#include "../Common/Perform.h"
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Utility/UtilityMath.h"
#include "./InputManager.h"
#include "./Camera.h"
#include "./ResourceManager.h"
#include "./SoundManager.h"

// 静的インスタンスの初期化
SceneManager* SceneManager::instance_ = nullptr;


void SceneManager::CreateInstance()
{
	/* インスタンス生成処理 */

	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Initialize();
}


SceneManager::SceneManager(void)
	: curSceneId_(SCENE_ID::NONE)
	, waitSceneId_(SCENE_ID::NONE)
	, camera_(nullptr)
	, deltaTime_(1.0f / 60.0f)
	, isSceneChanging_(false)
	, isDebugMode_(false)
	, isChoiceMove_(true)
	, shadowMapHandle_(-1)
{
	bool isDebug = false;

#ifdef _DEBUG
	//isDebug = true;
#endif

	isDebugMode_ = isDebug;
}

void SceneManager::Initialize(void)
{
	/* 初期化処理 */

	curSceneId_  = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	// フェード機能の初期化
	fader_ = std::make_unique<Fader>();
	fader_->Initialize();

	perform_ = std::make_unique<Perform>();
	perform_->Initialize();

	// カメラ
	camera_ = new Camera();
	camera_->Initialize();

	// 画面遷移中判定
	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// 3D用の設定
	_Initialize3D();

	// 初期シーンの設定
	DoChangeScene(curSceneId_);

}

void SceneManager::_Initialize3D(void)
{
	/* 3Dの初期化処理 */

	// 背景色
	constexpr UtilityCommon::Color BACKGROUND_COLOR = UtilityCommon::Color(255, 255, 255);

	// フォグの色と距離
	constexpr UtilityCommon::Color FOG_COLOR = UtilityCommon::Color(255, 255, 255);
	constexpr float FOG_START = 10000.0f;
	constexpr float FOG_END = 15000.0f;

	// ディレクショナルライトの方向
	constexpr VECTOR LIGHT_DIR = { 0.0f, -0.7f, 0.8f };


	// 背景色設定
	SetBackgroundColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b);

	// Zバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(TRUE);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);
	ChangeLightTypeDir(UtilityMath::VNormalize(LIGHT_DIR));

	// フォグ設定
	SetFogEnable(true);
	SetFogColor(FOG_COLOR.r, FOG_COLOR.g, FOG_COLOR.b);
	SetFogStartEnd(FOG_START, FOG_END);

	// シャドウマップ初期化
	InitializeShadow();
}
void SceneManager::InitializeShadow(void)
{
	/* シャドウマップの初期化処理 */

	constexpr int SHADOW_MAP_SIZE = 2056;
	constexpr VECTOR SHADOW_AREA_MIN = { -10000.0f, -1.0f, -10000.0f };
	constexpr VECTOR SHADOW_AREA_MAX = { 10000.0f, 5000.0f, 10000.0f };
	const VECTOR SHADOW_DIR = { 0.0f, 0.0f, 1.0f };
	
	// シャドウマップの作成
	shadowMapHandle_ = MakeShadowMap(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	SetShadowMapLightDirection(shadowMapHandle_, UtilityMath::VNormalize(SHADOW_DIR));
	SetShadowMapDrawArea(shadowMapHandle_, SHADOW_AREA_MIN, SHADOW_AREA_MAX);
}


void SceneManager::Update(void)
{
	/* 更新処理 */

	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	fader_->Update();

	if (isSceneChanging_)
	{
		// フェード状態の切替処理
		Fade();
	}

	else
	{
		perform_->Update();

		// 演出中はシーンの更新を行わない
		if (perform_->GetPerformStop()) { return; }

		scene_->Update();
	}

	camera_->Update();

#ifdef _DEBUG
	if (InputManager::GetInstance().IsTrgDown(InputManager::TYPE::DEBUG_MODE))
	{
		isDebugMode_ = !isDebugMode_;
	}
#endif
}

void SceneManager::Draw(void)
{
	/* 描画処理 */

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);
	
	// 画面を初期化
	ClearDrawScreen();

	// 描画先のグラフィック領域とカメラの設定
	SetDrawArea(0, 0, Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y);
	SetCameraScreenCenter(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y);

	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(shadowMapHandle_);
	
	perform_->BeforeDraw();

	// 各シーンの描画処理
	scene_->Draw();

	ShadowMap_DrawEnd();


	// カメラの描画前処理
	camera_->SetBeforeDraw();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	// シャドウマップへの描画の準備
	SetUseShadowMap(0, shadowMapHandle_);
	

	// 各シーンの描画処理
	scene_->Draw();

	perform_->Draw();
	
	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);

	camera_->DrawDebug();

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
	
	// 暗転・明転
	fader_->Draw();
}

void SceneManager::DestroyInstance(void)
{
	/* メモリ解放処理 */

	camera_->Release();
	delete camera_;

	DeleteShadowMap(shadowMapHandle_);

	delete instance_;
}

void SceneManager::ChangeScene(SCENE_ID _nextId)
{
	/* シーン遷移処理 */

	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = _nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;
}


void SceneManager::ResetDeltaTime(void)
{
	/* デルタタイムのリセット処理 */

	constexpr float DEFAULT_DELTA_TIME = 0.016f;

	deltaTime_ = DEFAULT_DELTA_TIME;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	/* シーン切替処理 */

	// シーンを変更する
	curSceneId_ = sceneId;

	// 現在のシーンを解放

	if (curSceneId_ == SCENE_ID::TITLE)
	{
		// BGM停止
		SoundManager::GetInstance().StopAllChoice(true);
		scene_ = std::make_unique<TitleScene>();
	}
	else if (curSceneId_ == SCENE_ID::GAME)
	{
		scene_ = std::make_unique<GameScene>();
	}

	// 各シーンの初期化
	scene_->Initialize();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;
}

void SceneManager::Fade(void)
{
	/* フェード状態の切替処理 */

	Fader::STATE fState = fader_->GetState();

	if (fader_->IsEnd())
	{
		if (fState == Fader::STATE::FADE_IN)
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}

		else if (fState == Fader::STATE::FADE_OUT)
		{
			/* 完全に暗転してからシーン遷移 */
			DoChangeScene(waitSceneId_);

			// 暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
	}
}
