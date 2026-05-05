#pragma once
#include <DxLib.h>
#include "./ColliderBase.h"
class Transform;

class ColliderLine : public ColliderBase
{
public:

	/// @brief コンストラクタ
	/// @param _tag 自身のカプセル
	/// @param _follow 追従対象
	/// @param _localPosStart 始点位置
	/// @param _localPosEnd 終点位置
	ColliderLine(TAG _tag, const Transform* _follow,
				 const VECTOR& _localPosStart, const VECTOR& _localPosEnd);

	
	~ColliderLine(void) override = default;


	/* ローカル座標を割り当て */
	void SetLocalPosStart(const VECTOR& _pos) { localPosStart_ = _pos; };
	void SetLocalPosEnd(const VECTOR& _pos) { localPosEnd_ = _pos; };

	/* ローカル座標の取得 */
	const VECTOR& GetLocalPosStart(void) const { return localPosStart_;  };
	const VECTOR& GetLocalPosEnd(void) const { return localPosEnd_;  };

	/* ワールド座標の取得 */
	VECTOR GetPosStart(void) const;
	VECTOR GetPosEnd(void) const;


protected:

	/// @brief デバッグ描画
	/// @param _color カプセルの色
	void DrawDebug(unsigned int _color) override;


private:

	// 線分の開始座標(ローカル)
	VECTOR localPosStart_;

	// 線分の終了座標(ローカル)
	VECTOR localPosEnd_;
};