#include "GameTimer.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"


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
	if (isTimeActive_)
	{
		// 時間減少
		curTimer_ -= SceneManager::GetInstance().GetDeltaTime();

		// 時間が0の時、false
		isTimeActive_ = (curTimer_ > 0.0f);
	}
}
void GameTimer::DrawTimer(void)
{
	int x = Application::SCREEN_HALF_X - 100;
	int arrayNum = 0;

	// 小数点以下の数値
	float frac = (curTimer_ - std::floor(curTimer_));


	// 100の位
	x += 150;
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
		UI_TEXT_SCALE, 0.0, timeText_[10], true);

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
	int arrayNum = 0;

	arrayNum = static_cast<int>(_time);
	int image = ((_time > 0.0f)
		? timeText_[arrayNum + 1]
		: _uiHandle);

	double scale = ((_time > 0.0f) ? 2.0 : 1.0);

	DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y - 100
		, scale, 0.0, image, true);

}