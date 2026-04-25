#include "UtilityMatrix.h"

MATRIX UtilityMatrix::GetMatrixRotateXYZ(const VECTOR& euler)
{
	MATRIX ret = MGetIdent();
	ret = MMult(ret, MGetRotX(euler.x));
	ret = MMult(ret, MGetRotY(euler.y));
	ret = MMult(ret, MGetRotZ(euler.z));
    return ret;
}

MATRIX UtilityMatrix::Multiplication(const MATRIX& child, const MATRIX& parent)
{
	return MMult(child, parent);
}

MATRIX UtilityMatrix::Multiplication(const VECTOR& childEuler, const VECTOR& parentEuler)
{
	MATRIX parent = UtilityMatrix::GetMatrixRotateXYZ(parentEuler);
	MATRIX child = UtilityMatrix::GetMatrixRotateXYZ(childEuler);
	return MMult(child, parent);
}
