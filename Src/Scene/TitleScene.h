#pragma once
#include "SceneBase.h"
#include <array>
#include <functional>
#include "../Application.h"
#include "../Object/Common/Transform.h"
#include "../Utility/UtilityCommon.h"

class TitleScene : public SceneBase
{

public:

	enum class TITLE_STATE
	{
		NONE = -1,
		SELECT_START,	// 選択状態に遷移
		GAME_END,

		SELECT_MOVE,	// 移動ステージを選択
		SELECT_GRAVITY,	// 重力ステージを選択
		SELECT_CANCEL	// 選択をキャンセル
	};


	TitleScene(void);
	~TitleScene(void) override = default;

	void Initialize(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;


private:
	
	// タイトル状態
	TITLE_STATE state_;

	// 決定したか否か
	bool isSelected_;


	// タイトルUI画像
	enum class TITLE_UI_IMAGE
	{
		SELECT_START, // 選択開始
		GAME_END,	  // ゲーム終了
		SELECT_CHARA, // 選択キャラ

		MAX
	};
	int titleUIHandle_[static_cast<int>(TITLE_UI_IMAGE::MAX)];

	// 選択UI画像
	enum class SELECT_UI_IMAGE
	{
		BACK, // 戻る

		MOVE_NOT_SELECT,	// 移動ステージ 選択不可
		GRAVITY_NOT_SELECT, // 重力ステージ 選択不可

		MOVE_SELECT,	// 移動ステージ 選択
		GRAVITY_SELECT, // 重力ステージ 選択

		MAX,
	};
	int selectUIHandle_[static_cast<int>(SELECT_UI_IMAGE::MAX)];

	// タイトル画像
	int titleImage_;
	int titleBackImage_;


	// タイトル状態更新処理
	std::function<void(void)> updateStateFunc_;

	/* 状態別の描画処理 */
	std::function<void(void)> drawFuncTitle_;
	std::function<void(void)> drawFuncSelect_;


	/* 状態別の更新処理 */
	void Update_SelectStart(void);
	void Update_GameEnd(void);
	void Update_SelectMove(void);
	void Update_SelectGravity(void);
	void Update_SelectCancel(void);

	/* 状態別の描画処理 */
	void Draw_SelectStart(void);
	void Draw_GameEnd(void);
	void Draw_SelectMove(void);
	void Draw_SelectGravity(void);
	void Draw_SelectCancel(void);

	/// @brief 状態遷移処理
	/// @param _selectUp 上入力後の状態
	/// @param _selectDown 下入力後の状態
	void ChangeStateProc(TITLE_STATE _selectUp, TITLE_STATE _selectDown);

	/// @brief 状態遷移処理の要素
	/// @param _state 状態遷移後の対象
	void _ChangeStateProc(TITLE_STATE _state);
};
