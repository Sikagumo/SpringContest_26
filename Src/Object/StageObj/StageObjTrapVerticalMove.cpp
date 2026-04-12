#include "./StageObjTrapVerticalMove.h"
#include "./StageObjBase.h"
#include "../Collider/ColliderSphere.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"
#include "../Stage/StageBase.h"

StageObjTrapVerticalMove::StageObjTrapVerticalMove(int _x, int _y, int _objType, float _alpha) :
	StageObjBase::StageObjBase(_x, _y, { COLLISION_SIZE_X, COLLISION_SIZE_Y }, _objType, _alpha)
{
}

void StageObjTrapVerticalMove::InitLoad(void)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_TRAP));

}

void StageObjTrapVerticalMove::InitTransform(void)
{
	transform_.InitTransform(BLOCK_SCALE,
		Quaternion::Identity(), Quaternion::AngleAxis(90.0f, AsoUtility::AXIS_Y));
}

void StageObjTrapVerticalMove::InitCollider(void)
{
	ColliderSphere* sphere = new ColliderSphere(ColliderBase::TAG::GOAL, &transform_, MODEL_OFFSET, COLLISION_RADIUS);
	ownColliders_.emplace(static_cast<int>(ColliderBase::SHAPE::SPHERE), sphere);
}

void StageObjTrapVerticalMove::Update(void)
{
    //移動先の座標を仮計算
    VECTOR nextPos = transform_.pos;
    float moveAmount = moveSpeed_ * static_cast<float>(moveDir_);
    nextPos.y += moveAmount;

    bool isHitWall = false;

    // 2. ステージの壁と衝突するかチェック
    if (stage_ != nullptr)
    {
        const auto& layout = stage_->GetPlaceType();
        for (const auto& row : layout)
        {
            for (auto* obj : row)
            {
                // 壁以外、または自分自身なら無視
                if (obj == nullptr || obj == this) continue;
                if (obj->GetObjType() != static_cast<int>(StageBase::BLOCK_TYPE::WALL)) continue;

                // 壁との距離判定 (球体判定)
                // 80.0f や 100.0f など、ブロックのサイズに合わせて調整してください
                if (AsoUtility::IsHitSpheres(nextPos, COLLISION_RADIUS, obj->GetTransform().pos, 100.0f))
                {
                    isHitWall = true;
                    break;
                }
            }
            if (isHitWall) break;
        }

        if (!isHitWall)
        {
            for (const auto& row : stage_->GetPlaceBackType())
            {
                for (auto* obj : row)
                {
                    if (obj == nullptr || obj == this) continue;
                    if (obj->GetObjType() != static_cast<int>(StageBase::BLOCK_TYPE::WALL)) continue;

                    // X軸の移動先に壁があるか判定 (判定距離は 80.0f 前後で調整)
                    if (AsoUtility::IsHitSpheres(nextPos, COLLISION_RADIUS, obj->GetTransform().pos, 80.0f))
                    {
                        isHitWall = true;
                        break;
                    }
                }
                if (isHitWall) break;
            }
        }
    }

    // 3. 判定結果に基づいて処理
    if (isHitWall)
    {
        // 壁にぶつかるなら反転する
        moveDir_ *= -1;
    }
    else
    {
        // ぶつからないなら移動を確定
        transform_.pos = nextPos;
    }

    // 4. 【重要】行列を更新（これをしないと描画位置がズレる、または動かない）
    transform_.Update();
}