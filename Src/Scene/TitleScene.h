#pragma once
#include "SceneBase.h"
#include <functional>
#include "../Application.h"
#include "../Object/Common/Transform.h"
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
		SELECT_CANSULE	// 選択をキャンセル
	};


	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void) override = default;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;


private:
	
	TITLE_STATE state_;
	bool isSelected_;

	std::function<void(void)> updateStateFunc_;
	std::function<void(void)> changeStateFunc_;

	void Update_SelectStart(void);
	void Update_GameEnd(void);
	void Update_SelectMove(void);
	void Update_SelectGravity(void);
	void Update_SelectCansule(void);

	void ChangeStateProc(TITLE_STATE _selectUp, TITLE_STATE _selectDown);
};
