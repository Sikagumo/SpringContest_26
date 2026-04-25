#include "ActorBase.h"
#include <vector>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../Collider/ColliderBase.h"

ActorBase::ActorBase(void)
	: resMng_(ResourceManager::GetInstance())
	, sceneMng_(SceneManager::GetInstance())
	, transform_(Transform())
{
}


void ActorBase::Init(void)
{
	/* 初期化処理 */

	// リソースロード
	InitLoad();

	// Transform初期化
	InitTransform();

	// 衝突判定の初期化an
	InitCollider();

	// 初期化後の個別処理
	InitPost();

}

void ActorBase::Draw(void)
{
	/* 描画処理 */

	// 前描画
	DrawPre();

	if (transform_.modelId != -1)
	{
		// モデル描画
		MV1DrawModel(transform_.modelId);
	}

	// 後描画
	DrawLate();

#ifdef _DEBUG
	
	if (sceneMng_.GetIsDebugMode())
	{
		// 所有しているコライダの描画
		for (const auto& [objType_, collider] : ownColliders_)
		{
			collider->Draw();
		}
	}
#endif
}

void ActorBase::Release(void)
{
	/* メモリの解放 */
	transform_.Release();

	// 自身のコライダ解放
	for (auto& own : ownColliders_)
	{
		delete own.second;
	}
}

const ColliderBase* ActorBase::GetOwnCollider(int key) const
{
	/* 衝突情報取得 */

	// 指定のコライダが無ければnullptrを返す
	if (ownColliders_.count(key) == 0)
	{
		return nullptr;
	}
	return ownColliders_.at(key);
}


void ActorBase::AddHitCollider(const ColliderBase* hitCollider)
{
	/* 衝突判定登録 */
	for (const auto& collider : hitColliders_)
	{
		// 衝突相手の登録
		if (collider == hitCollider) { return; }
	}
	hitColliders_.emplace_back(hitCollider);
}
void ActorBase::RemoveHitCollider(const ColliderBase::TAG _targetTag)
{
	/* 対象の衝突判定を削除 */

	const int SIZE = (static_cast<int>(hitColliders_.size()) - 1);
	for (int i = SIZE; i >= 0; i--)
	{
		// 指定のタグの当たり判定を外す
		if (hitColliders_[i]->GetTag() == _targetTag)
		{
			hitColliders_.erase(hitColliders_.begin() + i);
		}
	}
}

void ActorBase::ClearHitCollider(void)
{
	// 衝突相手の登録を全てクリア
	hitColliders_.clear();
}
