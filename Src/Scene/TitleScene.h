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

	static constexpr COLOR_U8 COLOR_SELECT = COLOR_U8(100, 50, 50);
	static constexpr COLOR_U8 COLOR_SELECT_NOT = COLOR_U8(200, 0, 0);

	struct SelectUI
	{
		Vector2* pos;
		unsigned int color;
	};
	std::array<SelectUI , 3 > selectUI_;

	// タイトル状態更新処理
	std::function<void(void)> updateStateFunc_;


	void Update_SelectStart(void);
	void Update_GameEnd(void);
	void Update_SelectMove(void);
	void Update_SelectGravity(void);
	void Update_SelectCancel(void);

	void ChangeStateProc(TITLE_STATE _selectUp, TITLE_STATE _selectDown);
};
