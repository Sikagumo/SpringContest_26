#pragma once
#include "../Actor/ActorBase.h"

class SkyDome : public ActorBase
{
public:

	enum class STATE
	{
		STOP,	// 回転/追従無し
		STAY,	// 追従無し
		FOLLOW, // 追従
	};

	/// @brief コンストラクタ
	/// @param _transform 追従対象
	SkyDome(const Transform& _transform);

	/// @brief デフォルトデストラクタ
	~SkyDome(void)override = default;

	/// @brief 更新処理
	void Update(void)override;

	/// @brief 描画処理
	void Draw(void)override;


protected:

	// リソースロード
	void InitLoad(void)override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void)override;

	// 衝突判定の初期化
	void InitCollider(void)override {};

	// 初期化後の個別処理
	void InitPost(void)override;


private:

	// 大きさ
	static constexpr float SCALE = 100.0f;

	// 回転速度
	static constexpr float ROT_SPEED = 0.005f;


	// 追従する対象
	const Transform& followTransform_;

	// 現在の状態
	STATE state_;


	/// @brief 状態遷移処理
	/// @param _state 遷移先の状態
	void ChangeState(STATE _state);

	/* 各更新処理 */
	void UpdateStay(void);
	void UpdateFollow(void);
};