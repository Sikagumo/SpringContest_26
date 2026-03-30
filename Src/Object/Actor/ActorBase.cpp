#include "ActorBase.h"
#include <vector>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../Collider/ColliderBase.h"

ActorBase::ActorBase(void)
	: 
	resMng_(ResourceManager::GetInstance()),
	sceneMng_(SceneManager::GetInstance()),
	transform_(Transform())
{
}
void ActorBase::Init(void)
{
	// リソースロード
	InitLoad();

	// Transform初期化
	InitTransform();

	// 衝突判定の初期化an
	InitCollider();

	// アニメーションの初期化
	InitAnimation();

	// 初期化後の個別処理
	InitPost();

}

void ActorBase::Draw(void)
{
	// 前描画
	DrawPre();

	if (transform_.modelId != -1)
	{
		MV1DrawModel(transform_.modelId);
	}

	// 後描画
	DrawLate();

#ifdef _DEBUG
	/*
	if (sceneMng_.GetIsDebugMode())
	{
		// 所有しているコライダの描画
		for (const auto& [type, collider] : ownColliders_)
		{
			collider->Draw();
		}
	}*/
#endif
}

void ActorBase::Release(void)
{
	transform_.Release();

	// 自身のコライダ解放
	for (auto& own : ownColliders_)
	{
		delete own.second;
	}
}

Transform& ActorBase::GetTransform()
{
	return transform_;
}

const ColliderBase* ActorBase::GetOwnCollider(int key) const
{
	if (ownColliders_.count(key) == 0)
	{
		return nullptr;
	}
	return ownColliders_.at(key);
}


void ActorBase::AddHitCollider(const ColliderBase* hitCollider)
{
	for (const auto& c : hitColliders_)
	{
		// 衝突相手の登録
		if (c == hitCollider) { return; }
	}
	hitColliders_.emplace_back(hitCollider);
}
void ActorBase::RemoveHitCollider(const ColliderBase::TAG _targetTag)
{
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
	hitColliders_.clear();
}
