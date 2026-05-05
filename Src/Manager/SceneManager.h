#pragma once
#include <chrono>
#include "../Utility/UtilityCommon.h"
#include <memory>
#include <DxLib.h>
class SceneBase;
class Fader;
class Camera;
class Perform;

class SceneManager
{

public:

	enum class SCENE_ID
	{
		NONE = -1,
		TITLE,
		GAME,
	};
	
	static void CreateInstance(void);
	static SceneManager& GetInstance(void) { return *instance_; };
	void DestroyInstance(void);

	void Initialize(void);
	void Update(void);
	void Draw(void);

	

	/// @brief 状態遷移
	/// @param _nextId 
	void ChangeScene(SCENE_ID _nextId);

	SCENE_ID GetCurSceneID(void) { return curSceneId_; };

	float GetDeltaTime(void) const { return deltaTime_; };

	Camera* GetCamera(void) const { return camera_; };

	/// @brief 演出の取得
	const std::unique_ptr<Perform>& GetPerform(void) { return perform_; }


	/// @brief デバッグ状態取得
	bool GetIsDebugMode(void) { return isDebugMode_; };

	/// @brief 移動ステージを選択するか否か
	/// @param _isChoiceMove 移動ステージを選択するか否か
	void SetIsStageMove(bool _isChoiceMove) { isChoiceMove_ = _isChoiceMove; };

	/// @brief 移動ステージを選択したか否か
	bool GetIsStageMove(void) { return isChoiceMove_; };


private:

	// 静的インスタンス
	static SceneManager* instance_;

	// シーンID
	SCENE_ID curSceneId_;
	SCENE_ID waitSceneId_;

	// フェード
	std::unique_ptr<Fader> fader_;

	// 演出
	std::unique_ptr<Perform> perform_;

	// 各種シーン
	std::unique_ptr<SceneBase> scene_;

	// カメラ
	Camera* camera_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	// デバッグモードか否か
	bool isDebugMode_;

	int shadowMapHandle_;

	// 移動ステージを選択したか否か
	bool isChoiceMove_;
	

	SceneManager(void);
	~SceneManager(void) = default;

	/* コピーコンストラクタ対策 */
	SceneManager(const SceneManager&)			 = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(SceneManager&&)			= delete;
	SceneManager& operator=(SceneManager&&) = delete;


	void _Initialize3D(void);
 
	void ResetDeltaTime(void);

	/// @brief シーン遷移の実行
	/// @param _sceneId 遷移先シーンID
	void DoChangeScene(SCENE_ID _sceneId);

	/// @brief フェード状態の切替処理
	void Fade(void);

	void InitializeShadow(void);
};