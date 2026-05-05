#include "./StageObjTrapHorizontalMove.h"
#include "./StageObjBase.h"
#include "../Collider/ColliderSphere.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/UtilityMath.h"
#include "../Stage/StageBase.h"

StageObjTrapHorizontalMove::StageObjTrapHorizontalMove(Vector2 _arrayPos, int _objType, float _alpha)
    : StageObjBase::StageObjBase(_arrayPos, VGet(COLLISION_SIZE, COLLISION_SIZE, COLLISION_SIZE), _objType, _alpha)
{
}

void StageObjTrapHorizontalMove::InitLoad(void)
{
    transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_TRAP));

}

void StageObjTrapHorizontalMove::InitTransform(void)
{
    transform_.InitTransform(BLOCK_SCALE,
        Quaternion::Identity(), Quaternion::AngleAxis(90.0f, UtilityMath::AXIS_Y));
}

void StageObjTrapHorizontalMove::InitCollider(void)
{
    ColliderSphere* sphere = new ColliderSphere(ColliderBase::TAG::GOAL, &transform_, MODEL_OFFSET, COLLISION_RADIUS);
    ownColliders_.emplace(static_cast<int>(ColliderBase::SHAPE::SPHERE), sphere);
}

void StageObjTrapHorizontalMove::Update(void)
{
    //次の移動先を「X軸」で仮計算
    VECTOR nextPos = transform_.pos;
    nextPos.x += moveSpeed_ * static_cast<float>(moveDir_);

    bool isHitWall = false;

    //ステージの壁との衝突チェック
    if (stage_ != nullptr)
    {
        for (const auto& row : stage_->GetPlaceType())
        {
            for (auto* obj : row)
            {
                if (obj == nullptr || obj == this) continue;
                if (obj->GetObjType() != static_cast<int>(StageBase::BLOCK_TYPE::WALL)) continue;

                // X軸の移動先に壁があるか判定 (判定距離は 80.0f 前後で調整)
                if (UtilityMath::IsHitSpheres(nextPos, COLLISION_RADIUS, obj->GetTransform().pos, 80.0f))
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
                    if (UtilityMath::IsHitSpheres(nextPos, COLLISION_RADIUS, obj->GetTransform().pos, 80.0f))
                    {
                        isHitWall = true;
                        break;
                    }
                }
                if (isHitWall) break;
            }
        }
    }

    //反転処理または座標更新
    if (isHitWall)
    {
        moveDir_ *= -1; // 壁に当たったら移動方向を反転
    }
    else
    {
        transform_.pos = nextPos; // ぶつからないなら横移動を確定
    }

    //行列の更新（描画位置に反映）
    transform_.Update();
}