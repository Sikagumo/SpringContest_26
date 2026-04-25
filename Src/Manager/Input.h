#pragma once
#include <map>
#include <vector>
#include <Dxlib.h>
#include "../Common/Vector2.h"

/* 入力判定用クラス */
class Input
{
public:

	// ゲームコントローラーの認識番号
	// DxLib定数、DX_INPUT_PAD1等に対応
	enum class JOYPAD_NO
	{
		KEY_PAD1,			// キー入力とパッド１入力
		PAD1,				// パッド１入力
		PAD2,				// パッド２入力
		PAD3,				// パッド３入力
		PAD4,				// パッド４入力
		INPUT_KEY = 4096,	// キー入力
		PAD_ALL,			// パッド１～４の入力
	};

	// ゲームコントローラータイプ
	// DxLib定数、DX_OTHER等に対応
	enum class JOYPAD_TYPE
	{
		OTHER = 0,
		XBOX_360,
		XBOX_ONE,
		DUAL_SHOCK_4,
		DUAL_SENSE,
		SWITCH_JOY_CON_L,
		SWITCH_JOY_CON_R,
		SWITCH_PRO_CTRL,
		MAX
	};

	// ゲームコントローラーボタン
	enum class JOYPAD_BTN
	{
		RB_LEFT = 0,	// X
		RB_RIGHT,		// B
		RB_TOP,			// Y
		RB_BOTTOM,		// A

		R_TRIGGER,
		L_TRIGGER,
		R_BUTTON,
		L_BUTTON,
		START,
		SELECT,

		/* 十字スティック */
		CROSS_TOP,
		CROSS_DOWN,
		CROSS_LEFT,
		CROSS_RIGHT,

		/* スティック押し込み */
		L_STICK,
		R_STICK,

		MAX
	};

	enum class JOYPAD_STICK
	{
		/* 左スティック */
		L_STICK_UP,
		L_STICK_DOWN,
		L_STICK_LEFT,
		L_STICK_RIGHT,

		/* 右スティック */
		R_STICK_UP,
		R_STICK_DOWN,
		R_STICK_LEFT,
		R_STICK_RIGHT,

		MAX
	};

	enum class MOUSE
	{
		NONE = -1,

		/* クリック */
		CLICK_RIGHT,
		CLICK_LEFT,
		CLICK_MIDDLE,

		/* 移動 */
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN,

		WHEEL_FRONT,  // ホイール前(奥)回転
		WHEEL_BACK,	  // ホイール後ろ(手前)回転

		MAX
	};

	/// @brief マウスカーソル状態
	enum class CURSOR_MODE
	{
		NONE,     // 変更なし
		LOCKED,   // 画面中央固定
		CONFINED, // ウィンドウ内
		CONFINED_SIDE, // ウィンドウ内の反対側に行く
	};

	// ゲームコントローラーの入力情報
	struct JOYPAD_IN_STATE
	{
		unsigned char ButtonsOld[static_cast<int>(JOYPAD_BTN::MAX)];
		unsigned char ButtonsNew[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsOld[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsNew[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsTrgDown[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsTrgUp[static_cast<int>(JOYPAD_BTN::MAX)];
		int AKeyLX;
		int AKeyLY;
		int AKeyRX;
		int AKeyRY;
	};


	/// @brief デフォルトコンストラクタ
	Input(void);

	/// @brief デフォルトデストラクタ
	~Input(void) = default;

	/// @brief 初期化処理
	void Init(void);

	/// @brief 更新処理 
	void Update(void);

	/// @brief メモリ開放処理
	void Release(void);

	/// @brief 指定するキーの追加
	void Add(int key);


	/// @brief キーの押下しているか否か
	bool IsNew(int key) const;

	/// @brief キーの押下したか否か(押しっぱなしはNG)
	bool IsTrgDown(int key) const;

	/// @brief キーを離したか否か
	bool IsTrgUp(int key) const;


	/// @brief マウス位置設定
	void SetMousePos(const Vector2& pos);

	/// @brief マウス座標の取得
	Vector2 GetMousePos(void) const { return mousePos_; };

	/// @brief マウスの移動距離を取得
	Vector2 GetMousePosDistance(void)const;

	/// @brief マウスのクリック状態を取得(MOUSE_INPUT_LEFT、RIGHT)
	int GetMouse(void) const { return mouseInput_; };

	/// @brief マウスが左クリックしたたか否か
	bool IsClickMouseLeft(void)const { return (mouseInput_ == MOUSE_INPUT_LEFT); };

	// マウスが右クリックしたか否か
	bool IsClickMouseRight(void) const { return (mouseInput_ == MOUSE_INPUT_RIGHT); };

	/// @brief カーソル状態変更
	void SetCursorMode(CURSOR_MODE _mode) { cursorMode_ = _mode; };

	/// @brief コントローラの入力情報を取得
	JOYPAD_IN_STATE GetJPadInputState(JOYPAD_NO no);

	/* ボタン入力したかの判定 */
	bool IsPadBtnNew(JOYPAD_NO no, JOYPAD_BTN btn) const;
	bool IsPadBtnTrgDown(JOYPAD_NO no, JOYPAD_BTN btn) const;
	bool IsPadBtnTrgUp(JOYPAD_NO no, JOYPAD_BTN btn) const;

	/* スティックが倒されたかの判定 */
	bool IsStickNew(JOYPAD_NO no, JOYPAD_STICK stick) const;
	bool IsStickDown(JOYPAD_NO no, JOYPAD_STICK stick) const;
	bool IsStickUp(JOYPAD_NO no, JOYPAD_STICK stick) const;

	/* マウスが入力されたかの判定 */
	bool IsMouseNew(MOUSE mouse) const;
	bool IsMouseTrgUp(MOUSE mouse) const;
	bool IsMouseTrgDown(MOUSE mouse) const;


private:

	//スティックの猶予
	static constexpr int STICK_THRESHOLD = 300;

	// キー情報
	struct KeyInfo
	{
		int key;		 // キーID
		bool keyOld;	 // 1フレーム前の押下状態
		bool keyNew;	 // 現フレームの押下状態
		bool keyTrgDown; // 現フレームでボタンが押されたか
		bool keyTrgUp;	 // 現フレームでボタンが離されたか
	};

	// スティック情報
	struct StickInfo
	{
		JOYPAD_STICK key; // スティックの種類
		bool keyOld = false; // 1フレーム前の傾き状態
		bool keyNew = false; // 現フレームの傾き状態
		bool keyTrgDown = false; // 現フレームでスティックがニュートラルから倒されたか
		bool keyTrgUp   = false; // 現フレームでスティックがニュートラルに戻ったか
	};

	// マウス
	struct MouseInfo
	{
		bool keyOld;	 // 1フレーム前の押下状態
		bool keyNew;	 // 現フレームの押下状態
		bool keyTrgDown; // 現フレームでボタンが押されたか
		bool keyTrgUp;	 // 現フレームでボタンが離されたか
	};


	// コントローラ情報
	DINPUT_JOYSTATE joyDInState_;

	// コントローラ情報(XBOX)
	XINPUT_STATE joyXInState_;

	// キー情報
	std::map<int, Input::KeyInfo> keyInfos_;
	Input::KeyInfo infoEmpty_;

	// マウス情報
	std::map<MOUSE, Input::MouseInfo> mouseInfos_;
	Input::MouseInfo mouseInfoEmpty_;

	// スティック情報
	std::map<JOYPAD_NO, std::vector<Input::StickInfo>> stickInfos_;

	// カーソル状態
	CURSOR_MODE cursorMode_;
	
	// マウスカーソルの位置
	Vector2 mousePrePos_;
	Vector2 mousePos_;

	// マウスホイール回転量
	int wheelRot_;

	// マウスボタンの入力状態
	int mouseInput_;

	// パッド情報
	JOYPAD_IN_STATE padInfos_[5];


	/// @brief マウスの入力情報を更新
	void UpdateMouse(void);

	/// @brief 配列の中からキー情報を取得
	const Input::KeyInfo& Find(int key) const;

	/// @brief 配列の中からマウス情報を取得
	const Input::MouseInfo& FindMouse(MOUSE key) const;

	/// @brief スティックの倒れ具合を取得
	int PadStickOverSize(const JOYPAD_NO no, const JOYPAD_STICK stick);

	/// @brief 接続されたコントローラの種別を取得
	JOYPAD_TYPE GetJPadType(JOYPAD_NO no);

	/// @brief コントローラの入力情報を取得
	DINPUT_JOYSTATE GetJPadDInputState(JOYPAD_NO no);

	/// @brief コントローラ(XBOX)の入力情報を取得
	XINPUT_STATE GetJPadXInputState(JOYPAD_NO no);

	/// @brief コントローラの入力情報を更新
	void SetJPadInState(JOYPAD_NO jpNo);

};

