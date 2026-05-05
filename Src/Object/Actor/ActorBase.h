#pragma once
#include "../Common/Transform.h"
#include "../Collider/ColliderBase.h"
#include <map>
#include <vector>
class ColliderBase;
class ResourceManager;
class SceneManager;

class ActorBase
{
public:

	using ColliderMap = std::map<int, ColliderBase*>;

	ActorBase(void);
	virtual ~ActorBase(void) = default;

	void Initialize(void);
	virtual void Update(void) = 0;
	virtual void Draw(void);
	virtual void Release(void);


	/// @brief 大きさ、回転、座標等の取得
	const Transform& GetTransform(void) const { return transform_; };

	/// @brief 自身の衝突情報取得
	const ColliderMap& GetOwnColliders(void) const { return ownColliders_; }

	/// @brief 特定の自身の衝突情報取得
	/// @param _key 
	const ColliderBase* GetOwnCollider(int _key) const;

	/// @brief 衝突対象となるコライダを登録
	/// @param hitCollider 対象のコライタ
	void AddHitCollider(const ColliderBase* _hitCollider);

	/// @brief 指定したタグがついた衝突対象となるコライダを除外
	/// @param _targetTag 対象のタグ
	void RemoveHitCollider(const ColliderBase::TAG _targetTag);

	/// @brief 衝突対象となるコライダをクリア
	void ClearHitCollider(void);


protected:

	// マネージャ参照
	ResourceManager& resMng_;
	SceneManager& sceneMng_;

	// モデル制御の基本情報
	Transform transform_;

	// 自身の衝突情報
	ColliderMap ownColliders_;

	// 衝突相手の情報リスト
	std::vector<const ColliderBase*> hitColliders_;


	/// @brief リソースロード
	virtual void InitLoad(void) = 0;

	/// @brief 大きさ、回転、座標の初期化
	virtual void InitTransform(void) = 0;

	/// @brief 衝突判定の初期化
	virtual void InitCollider(void) = 0;

	/// @brief 初期化後の個別処理
	virtual void InitPost(void) = 0;

	/// @brief 前描画
	virtual void DrawPre(void) {};

	/// @brief 後描画
	virtual void DrawLate(void) {};
};
