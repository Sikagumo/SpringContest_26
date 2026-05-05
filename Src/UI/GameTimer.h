#pragma once

class GameTimer
{
public:

	/// @brief コンストラクタ
	/// @param _gameTime ゲーム最大時間
	GameTimer(float _gameTime);

	void Update(void);
	void Draw(void);


	/// @brief カウントダウン描画処理
	/// @param _time カウントダウン時間
	/// @param _startTextUi げーむすたーとUI画像ハンドル
	void DrawCountDown(float _time, int _startTextUi);

	/// @brief 現在時間取得
	float GetTime(void)const { return curTimer_; };

	/// @brief 時間を動かすか否か割り当て
	void SetIsTimeActive(bool _isActive) { isTimeActive_ = _isActive; };

	/// @brief 時間を動かしているか否か
	bool GetIsTimeActive(void)const { return isTimeActive_; };


private:

	// 数字の数(0～9)+ドット
	static constexpr int TIME_NUM = 11;

	// 画像のドットの格納番地
	static constexpr int TIME_DOT = (TIME_NUM - 1);


	// 数字の画像
	int timeText_[TIME_NUM];

	// 現在時間
	float curTimer_;

	// 時間を変動させるか否か
	bool isTimeActive_;

};