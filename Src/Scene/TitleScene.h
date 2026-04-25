#pragma once
#include "SceneBase.h"
#include <array>
#include <functional>
#include "../Application.h"
#include "../Object/Common/Transform.h"
#include "../Utility/UtilityCommon.h"
class AnimationController;

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


	/// @brief コンストラクタ  
	TitleScene(void);

	/// @brief デフォルトデストラクタ
	~TitleScene(void) override = default;

	/// @brief 初期化
	void Init(void) override;

	/// @brief 更新
	void Update(void) override;

	/// @brief 描画
	void Draw(void) override;

	/// @brief 解放
	void Release(void) override;


private:
	
	// タイトル状態
	TITLE_STATE state_;

	// 決定したか否か
	bool isSelected_;

	static constexpr float TITLE_UI_SCALE = (1.0f - 0.0f);
	static constexpr float TITLE_NOT_UI_SCALE = (1.0f - 0.25f);

	static constexpr float SELECT_UI_SCALE = (1.0f - 0.15f);
	static constexpr float SELECT_NOT_UI_SCALE = (1.0f - 0.6f);

	static constexpr int TITLE_UI_OFFSET = 250;
	static constexpr int TITLE_UI_OFFSET_Y = 75;
	static constexpr int SELECT_UI_OFFSET = 500;

	static constexpr int TITLE_NOT_SUB = (255 - 200);
	static constexpr int SELECT_ALPHA = (255 - 100);

	enum class TITLE_UI_IMAGE
	{
		SELECT_START,
		GAME_END,
		SELECT_CHARA,

		MAX
	};
	int titleUIHandle_[static_cast<int>(TITLE_UI_IMAGE::MAX)];

	enum class SELECT_UI_IMAGE
	{
		BACK,
		MOVE_NOT_SELECT,
		GRAVITY_NOT_SELECT,

		MOVE_SELECT,
		GRAVITY_SELECT,

		MAX,
	};
	int selectUIHandle_[static_cast<int>(SELECT_UI_IMAGE::MAX)];

	// タイトル画像
	int titleImage_;
	int titleBackImage_;

	static constexpr int BACK_HALF_X = (2040 / 2) * 2;
	
	std::array<Vector2F, 3> backImagesPos_;


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
