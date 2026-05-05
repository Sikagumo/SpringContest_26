#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Object/Actor/ActorBase.h"
class Transform;

class Camera : public ActorBase
{

public:

	// カメラモード
	enum class MODE
	{
		NONE = -1,
		FREE,		 // 手動移動モード
		FIXED_POINT, // 定点カメラモード
		FOLLOW,		 // 追尾モード
	};

	// 衝突判定種別
	enum class COLLIDER_TYPE
	{
		SPHERE,
		MAX,
	};

	// カメラの初期座標
	static constexpr VECTOR DERFAULT_POS = { 0.0f, 0.0f, -1000.0f };

	// カメラの初期角度
	static constexpr VECTOR DERFAULT_ANGLES = { 0.0f, 0.0f, 0.0f };


	// カメラの回転量
	const float ROT_POW_DEG = 0.0f;
	const float ROT_POW_RAD = ROT_POW_DEG * (DX_PI_F / 180.0f);

	// カメラの移動スピード
	static constexpr float ROT_SPEED = 50.0f;

	// カメラのクリップ範囲
	static constexpr float VIEW_NEAR = 1.0f;
	static constexpr float VIEW_FAR = 25000.0f;

	// 追従位置からカメラ位置までの相対座標
	static constexpr VECTOR FOLLOW_CAMERA_LOCAL_POS = { 0.0f, 250.0f, 0.0f };

	// 追従位置から注視点までの相対座標
	static constexpr VECTOR FOLLOW_TARGET_LOCAL_POS = { 0.0f, 0.0f, 500.0f };

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = (40.0f * (DX_PI_F / 180.0f));
	static constexpr float LIMIT_X_DW_RAD = (10.0f * (DX_PI_F / 180.0f));


	Camera(void);
	~Camera(void)override = default;

	void Update(void)override;
	void DrawDebug(void);
	void Release(void)override;

	// 描画前のカメラ設定
	void SetBeforeDraw(void);
	

	const VECTOR& GetPos(void) const { return transform_.pos; };

	const VECTOR& GetAngles(void) const { return angles_;  };
	const Quaternion& GetQuaRot(void) const { return transform_.quaRot; };
	
	/// @brief X回転を抜いたY軸のみのカメラ角度
	const Quaternion& GetQuaRotY(void) const { return rotY_; };
	
	/// @brief 注視点の取得
	const VECTOR& GetTargetPos(void) const { return targetPos_;  };

	/// @brief カメラの前方方向
	VECTOR GetForward(void) const;

	/// @brief カメラモードの変更
	void ChangeMode(MODE _mode);

	/// @brief 追従対象の設定
	void SetFollow(const Transform* _follow) { followTransform_ = _follow; };


protected:

	/// @brief リソースロード
	void InitLoad(void) override {}

	/// @brief 大きさ、回転、座標の初期化
	void InitTransform(void) override {}

	/// @brief 衝突判定の初期化
	void InitCollider(void) override;

	/// @brief 初期化後の個別処理
	void InitPost(void) override;


private:


	// カメラの更新前位置
	VECTOR prePos_;

	// カメラが追従対象とするTransform
	const Transform* followTransform_;

	// カメラモード
	MODE mode_;

	// カメラ角度(rad)
	VECTOR angles_;

	// カメラ角度(Y軸のみ)
	Quaternion rotY_;

	// 注視点
	VECTOR targetPos_;
	

	/// @brief カメラを初期位置に戻す
	void SetDefault(void);

	/// @brief 追従対象との位置同期を取る
	void SyncFollow(void);

	/// @brief カメラ操作
	void ProcessMove(void);

	/* モード別更新ステップ */
	void SetBeforeDrawFixed_Point(void);
	void SetBeforeDraw_Free(void);
	void SetBeforeDraw_Follow(void);

	/// @brief 衝突判定
	void Collision(void);

};
