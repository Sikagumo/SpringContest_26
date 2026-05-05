#pragma once
#pragma once
#include <DxLib.h>
class Transform;

class ColliderBase
{
public:

	// 形状
	enum class SHAPE
	{
		NONE = -1,

		LINE,	 // 線分
		SPHERE,	 // 球
		CAPSULE, // カプセル
		MODEL,	 // モデル
		PLANE,	 // 平面
	};

	// 衝突種別
	enum class TAG
	{
		STAGE,	// ステージ
		PLAYER, // プレイヤー
		CAMERA, // カメラ
		GOAL,	// ゴール
	};


	/// @brief コンストラクタ
	/// @param _shape 
	/// @param _tag タグ
	/// @param _follow 
	ColliderBase(SHAPE _shape, TAG _tag, const Transform* _follow);
	virtual ~ColliderBase(void) = default;

	void Draw(void);

	/// @brief 追従先の取得
	const Transform* GetFollow(void) const { return follow_; };

	/// @brief 追従先の再設定処理
	void SetFollow(Transform* _follow) { follow_ = _follow; };

	/// @brief 形状の取得
	SHAPE GetShape(void) const { return shape_; }

	/// @brief 衝突種別の取得
	TAG GetTag(void) const { return tag_; }


protected:

	// 形状
	SHAPE shape_;

	// 衝突種別
	TAG tag_;

	// 追従先
	const Transform* follow_;

	// 有効フラグ
	bool isValid_;


	/// @brief ローカル座標をワールド座標に変換
	VECTOR GetRotPos(const VECTOR& _localPos) const;

	/// @brief デバッグ用描画
	virtual void DrawDebug(unsigned int _color) = 0;
};
