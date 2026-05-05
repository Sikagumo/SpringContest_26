#include "SkyDome.h"
#include <Dxlib.h>
#include "../Actor/ActorBase.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/UtilityMath.h"


SkyDome::SkyDome(void)
	: ActorBase()
{
}

void SkyDome::InitLoad(void)
{
	// モデルのロード
	transform_.SetModel(resMng_.LoadHandleId(ResourceManager::SRC::MODEL_SKYDOME));
}

void SkyDome::InitTransform(void)
{
	// 大きさ
	constexpr float SCALE = 100.0f;
	constexpr float LOCAL_LOT_Y = 180.0f;

	// 大きさ、回転、座標の初期化
	transform_.InitTransform(SCALE,
							 Quaternion::Identity(), Quaternion::AngleAxis(LOCAL_LOT_Y, UtilityMath::AXIS_Y),
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
	/* 更新処理 */

	// 回転速度
	constexpr float ROT_SPEED = 0.005f;

	// 回転
	transform_.Rotate(UtilityMath::AXIS_Y, ROT_SPEED);
}

void SkyDome::Draw(void)
{
	/* ライト効果を無効化して描画 */

	SetUseLighting(FALSE);
	ActorBase::Draw();
	SetUseLighting(TRUE);
}