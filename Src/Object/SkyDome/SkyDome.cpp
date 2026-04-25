#include "SkyDome.h"
#include <Dxlib.h>
#include "../Actor/ActorBase.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/UtilityMath.h"

SkyDome::SkyDome(const Transform& _transform)
	: ActorBase()
	, followTransform_(_transform)
	, state_(STATE::STOP)
{
}

void SkyDome::InitLoad(void)
{
	transform_.SetModel(resMng_.LoadHandleId(ResourceManager::SRC::MODEL_SKYDOME));
}
void SkyDome::InitTransform(void)
{
	transform_.InitTransform(SCALE,
							 Quaternion::Identity(), Quaternion::AngleAxis(180.0f, UtilityMath::AXIS_Y),
							 UtilityMath::VECTOR_ZERO);
}

void SkyDome::InitPost(void)
{
	// Zバッファの使用&書き込み無効(突き抜け対策)
	MV1SetUseZBuffer(transform_.modelId, false);
	MV1SetWriteZBuffer(transform_.modelId, false);
}

void SkyDome::Update(void)
{
	if (state_ == STATE::FOLLOW)
	{
		UpdateFollow();
	}

	// 
	else if (state_ == STATE::STAY)
	{
		UpdateStay();
	}
}
void SkyDome::UpdateStay(void)
{
	// 回転処理
	transform_.Rotate(UtilityMath::AXIS_Y, ROT_SPEED);
}
void SkyDome::UpdateFollow(void)
{
	// 回転処理
	transform_.Rotate(UtilityMath::AXIS_Y, ROT_SPEED);

	// 追従処理
	transform_.pos = followTransform_.pos;

	// 更新
	transform_.Update();
}


void SkyDome::Draw(void)
{
	/* ライト効果を無効化して描画 */
	SetUseLighting(FALSE);
	ActorBase::Draw();
	SetUseLighting(TRUE);
}

void SkyDome::ChangeState(STATE _state)
{
	state_ = _state;

	if (state_ != STATE::STOP)
	{
		// 回転停止状態以外時、追従対象の座標に移動
		transform_.pos = followTransform_.pos;
	}
}