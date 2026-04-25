#pragma once

class GameTimer
{
public:

	GameTimer(float _gameTime);

	void Update(void);

	void DrawTimer(void);

	void DrawCountDown(float _time, int _uiHandle);

	float GetTime(void)const { return curTimer_; };

	void SetIsTimeActive(bool _isActive) { isTimeActive_ = _isActive; };

	bool GetIsTimeActive(void)const { return isTimeActive_; };


private:

	static constexpr float UI_TEXT_SCALE = 0.5f;
	static constexpr int UI_TEXT_SIZE = static_cast<int>((80 * UI_TEXT_SCALE));

	// ”š‚Ì‰æ‘œ
	int timeText_[11];

	// Œ»İŠÔ
	float curTimer_;

	// ŠÔ‚ğ•Ï“®‚³‚¹‚é‚©”Û‚©
	bool isTimeActive_;

};