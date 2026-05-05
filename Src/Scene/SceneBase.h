#pragma once
#include <array>
class ResourceManager;
class SceneManager;
class InputManager;
class SoundManager;

class SceneBase
{

public:

	SceneBase(void);
	virtual ~SceneBase(void) = 0;

	virtual void Initialize(void) = 0;
	virtual void Update(void)	  = 0;
	virtual void Draw(void)		  = 0;
	virtual void Release(void)	  = 0;


protected:

	enum class UI_TEXT
	{
		PAUSE_MAIN,	  // 一時停止のメインテキスト
		PAUSE_SUB,	  // 一時停止のサブテキスト
		DECISION_SUB, // 決定のサブテキスト
		GAME_START,	  // ゲーム開始
		GAME_END,	  // ゲーム終了
		GAME_OVER,	  // ゲームオーバー
		GAME_CLEAR,	  // ゲームクリア
		STAGE_SELECT, // ステージ選択
		TIME_LIMIT,	  // タイムリミット
		TIME_CLEAR,	  // タイムクリア
		HIGH_SCORE,	  // ハイスコア

		MAX
	};
	std::array<int, static_cast<int>(UI_TEXT::MAX)> uiText_;

	// リソース管理
	ResourceManager& resMng_;

	// シーン管理
	SceneManager& sceneMng_;

	// 入力管理
	InputManager& input_;

	// 音声管理
	SoundManager& sound_;
};
