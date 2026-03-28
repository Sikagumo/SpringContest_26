#pragma once
#include "SceneBase.h"
#include <array>
#include <functional>
#include "../Application.h"
#include "../Object/Common/Transform.h"
#include "../Utility/UtilityCommon.h"
#include "../Common/Vector2.h"
class AnimationController;
class SkyDome;

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

	void ChangeTitleState(TITLE_STATE _state);


private:
	
	// タイトル状態
	TITLE_STATE state_;

	// 決定したか否か
	bool isSelected_;

	static constexpr float SELECT_UI_SCALE = (1.0f - 0.0f);
	static constexpr float SELECT_NOT_UI_SCALE = (1.0f - 0.5f);

	static constexpr int SELECT_UI_OFFSET = 500;

	static constexpr int SELECT_NOT_SUB = (255 - 200);

	enum class SELECT_IMAGE
	{
		BACK,
		MOVE_NOT_SELECT,
		GRAVITY_NOT_SELECT,

		MOVE_SELECT,
		GRAVITY_SELECT,

		MAX,
	};

	struct SelectUI
	{
		Vector2* pos;
		unsigned int color;
	};

	std::array<SelectUI, 3> selectUIParam_;

	int selectUIHandle_[static_cast<int>(SELECT_IMAGE::MAX)];

	// タイトル状態更新処理
	std::function<void(void)> updateStateFunc_;

	// 状態別描画処理
	std::function<void(void)> drawStateFunc_;


	void Update_SelectStart(void);
	void Update_GameEnd(void);
	void Update_SelectMove(void);
	void Update_SelectGravity(void);
	void Update_SelectCancel(void);

	void Draw_SelectMove(void);
	void Draw_SelectGravity(void);
	void Draw_SelectCancel(void);

	void ChangeStateProc(TITLE_STATE _selectUp, TITLE_STATE _selectDown);
};
