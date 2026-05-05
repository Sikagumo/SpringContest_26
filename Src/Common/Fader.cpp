#include "Fader.h"
#include <DxLib.h>
#include "../Application.h"

Fader::Fader(void)
	: state_(STATE::NONE)
	, alpha_(0.0f)
	, isPreEnd_(true)
	, isEnd_(true)
{
}


void Fader::Initialize(void)
{
}

void Fader::Update(void)
{
	// フェード終了時or無効状態時、処理終了
	if (isEnd_ || state_ == STATE::NONE) { return; }


	/* フェードアウト処理 */
	if (state_ == STATE::FADE_OUT)
	{
		alpha_ += SPEED_ALPHA;

		if (alpha_ > 255)
		{
			// フェード終了
			alpha_ = 255;

			if (isPreEnd_)
			{
				// 1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
	}

	/* フェードイン処理 */
	else if (state_ == STATE::FADE_IN)
	{
		alpha_ -= SPEED_ALPHA;
		if (alpha_ < 0)
		{
			// フェード終了
			alpha_ = 0;
			if (isPreEnd_)
			{
				// 1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
	}
}

void Fader::Draw(void)
{
	if (state_ == STATE::NONE) { return; }

	/* 透過描画 */

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);

	DrawBox(0, 0,
			Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
			0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


void Fader::SetFade(STATE state)
{
	state_ = state;

	if (state_ != STATE::NONE)
	{
		isPreEnd_ = false;
		isEnd_ = false;
	}
}