#include "GameTimer.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"

constexpr int TIME_POS = (Application::SCREEN_HALF_X - 100);

GameTimer::GameTimer(float _gameTime)
	: curTimer_(_gameTime)
	, isTimeActive_(false)
{
	for (int& time : timeText_) { time = -1; }

	// 複数画像割り当て
	ResourceManager::GetInstance().LoadHandleIds(ResourceManager::SRC::IMGS_TEXT_TIME, timeText_);
}

void GameTimer::Update(void)
{
	/* 更新処理 */
	if (isTimeActive_)
	{
		// 時間減少
		curTimer_ -= SceneManager::GetInstance().GetDeltaTime();

		// 時間が0の時、false
		isTimeActive_ = (curTimer_ > 0.0f);
	}
}
void GameTimer::Draw(void)
{
	/* 描画処理 */
	constexpr float UI_TEXT_SCALE = 0.5f;
	constexpr int UI_TEXT_SIZE = static_cast<int>((80 * UI_TEXT_SCALE));
	constexpr int TIME_OFFSET = 150;

	// 描画する数の位
	int arrayNum = 0;
	int x = TIME_POS;

	// 小数点以下の数値
	float frac = (curTimer_ - std::floor(curTimer_));


	// 100の位
	x += TIME_OFFSET;
	arrayNum = static_cast<int>(curTimer_ / 100.0f);
	if (arrayNum > 0)
	{
		x += UI_TEXT_SIZE;
		DrawRotaGraph(x, UI_TEXT_SIZE,
			UI_TEXT_SCALE, 0.0, timeText_[arrayNum], true);
	}

	// 10の位
	x += UI_TEXT_SIZE;
	arrayNum = static_cast<int>(curTimer_ / 10) % 10;
	DrawRotaGraph(x, UI_TEXT_SIZE,
		UI_TEXT_SCALE, 0.0, timeText_[arrayNum], true);

	// 1の位
	x += UI_TEXT_SIZE;
	arrayNum = static_cast<int>(curTimer_) % 10;
	DrawRotaGraph(x, UI_TEXT_SIZE,
		UI_TEXT_SCALE, 0.0, timeText_[arrayNum], true);

	// 小数点
	x += UI_TEXT_SIZE;
	arrayNum = static_cast<int>(curTimer_) % 10;
	DrawRotaGraph(x, UI_TEXT_SIZE,
		UI_TEXT_SCALE, 0.0, timeText_[TIME_DOT], true);

	// 第1小数点
	x += UI_TEXT_SIZE;
	arrayNum = static_cast<int>(frac * 10) % 10;
	DrawRotaGraph(x, UI_TEXT_SIZE,
		UI_TEXT_SCALE, 0.0, timeText_[arrayNum], true);

	// 第2小数点
	x += UI_TEXT_SIZE;
	arrayNum = static_cast<int>(frac * 100) % 10;
	DrawRotaGraph(x, UI_TEXT_SIZE,
		UI_TEXT_SCALE, 0.0, timeText_[arrayNum], true);

}

void GameTimer::DrawCountDown(float _time, int _uiHandle)
{
	/* カウントダウン描画処理 */

	// 画像スケール(カウントが０の場合は2倍、[げーむすたーと]描画時、1倍)
	const double SCALE = ((_time > 0.0f) ? 2.0 : 1.0);

	// 現在時間
	int arrayNum = static_cast<int>(_time);

	// カウントが0の時以外は[現在カウント+1]0未満では[げーむすたーと]を描画
	int image = ((_time > 0.0f)
		? timeText_[arrayNum + 1]
		: _uiHandle);


	DrawRotaGraph(Application::SCREEN_HALF_X, TIME_POS
				  , SCALE, 0.0, image, true);

}