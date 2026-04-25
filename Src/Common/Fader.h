#pragma once

class Fader
{

public:

	// フェードが進む速さ
	static constexpr float SPEED_ALPHA = 5.0f;

	// 状態
	enum class STATE
	{
		NONE, 
		FADE_OUT,	// 徐々に暗転
		FADE_IN		// 徐々に明転
	};


	/// @brief デフォルトコンストラクタ
	Fader(void);

	/// @brief デフォルトデストラクタ
	~Fader(void) = default;

	/// @brief フェード状態の取得
	STATE GetState(void) const { return state_; };

	/// @brief フェード処理が終了しているか否か
	bool IsEnd(void) const { return isEnd_; };

	/// @brief 指定フェードを開始する
	/// @param _state 指定する状態
	void SetFade(STATE _state);

	/// @brief 初期化
	void Init(void);

	/// @brief 更新
	void Update(void);

	/// @brief 描画
	void Draw(void);


private:

	// 状態
	STATE state_;

	// 透明度
	float alpha_;

	// 状態(STATE)を保ったまま終了判定を行うため、
	// Update->Draw->Updateの1フレーム判定用
	bool isPreEnd_;

	// フェード処理の終了判定
	bool isEnd_;

};
