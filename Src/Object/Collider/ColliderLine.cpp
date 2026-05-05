#include "ColliderLine.h"

ColliderLine::ColliderLine(TAG tag, const Transform* follow,
						   const VECTOR& localPosStart, const VECTOR& localPosEnd)
	: ColliderBase(SHAPE::LINE, tag, follow)
	, localPosStart_(localPosStart)
	, localPosEnd_(localPosEnd)
{
}


VECTOR ColliderLine::GetPosStart(void) const
{
	return GetRotPos(localPosStart_);
}
VECTOR ColliderLine::GetPosEnd(void) const
{
	return GetRotPos(localPosEnd_);
}

void ColliderLine::DrawDebug(unsigned int _color)
{
	/* デバッグ表示 */

	constexpr float RADIUS = 5.0f;

	// 球体ポリゴン分割数
	constexpr int DIV_NUM = 6;


	const VECTOR start = GetPosStart();
	const VECTOR end = GetPosEnd();

	// 線分を描画
	DrawLine3D(start, end, _color);

	// 始点・終点を球体で補助表示
	DrawSphere3D(start, RADIUS, DIV_NUM, _color, _color, true);
	DrawSphere3D(end, RADIUS, DIV_NUM, _color, _color, true);
}