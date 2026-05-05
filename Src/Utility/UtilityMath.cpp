#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <DxLib.h>
#include "UtilityMath.h"


int UtilityMath::Round(float _value)
{
    return static_cast<int>(roundf(_value));
}

std::vector<std::string> UtilityMath::Split(std::string& _line, char _delimiter)
{

    std::istringstream stream(_line);
    std::string field;
    std::vector<std::string> result;

    while (getline(stream, field, _delimiter)) {
        result.push_back(field);
    }

    return result;
}


double UtilityMath::DegIn360(double _deg)
{
    _deg = fmod(_deg, 360.0);
    if (_deg < 0.0f)
    {
        _deg += 360.0;
    }
    return _deg;
}

double UtilityMath::RadIn2PI(double _rad)
{
    _rad = fmod(_rad, DX_TWO_PI);
    if (_rad < 0.0)
    {
        _rad += DX_TWO_PI;
    }
    return _rad;
}

int UtilityMath::DirNearAroundRad(float _from, float _to)
{

    float ret = 1.0f;

    float diff = (_to - _from);

    if (diff >= 0.0f)
    {

        // 比較元よりも時計回りに位置する

        if (diff > DX_PI_F)
        {
            // でも、180度以上離れているので、反時計回りの方が近い
            ret = -1.0f;
        }
        else
        {
            // 時計回り
            ret = 1.0f;
        }

    }
    else
    {

        // 比較元よりも反時計回りに位置する

        if (diff < -DX_PI_F)
        {
            // でも、180度以上離れているので、時計回りの方が近い
            ret = 1.0f;
        }
        else
        {
            // 反時計回り
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

int UtilityMath::DirNearAroundDeg(float _from, float _to)
{

    float ret = 1.0f;

    float diff = (_to - _from);

    if (diff >= 0.0f)
    {

        // 比較元よりも時計回りに位置する

        if (diff > 180.0f)
        {
            // でも、180度以上離れているので、反時計回りの方が近い
            ret = -1.0f;
        }
        else
        {
            // 時計回り
            ret = 1.0f;
        }

    }
    else
    {

        // 比較元よりも反時計回りに位置する

        if (diff < -180.0f)
        {
            // でも、180度以上離れているので、時計回りの方が近い
            ret = 1.0f;
        }
        else
        {
            // 反時計回り
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

float UtilityMath::Clamp(float _value, float _min, float _max)
{
	_value = ((_value > _max) ? _max : _value);
	_value = ((_value < _min) ? _min : _value);
    return _value;
}
VECTOR UtilityMath::Clamp(const VECTOR& _target, const VECTOR& _min, const VECTOR& _max)
{
    // 数値の範囲制限
    VECTOR ret = _target;
    ret.x = std::clamp(ret.x, _min.x, _max.x);
    ret.y = std::clamp(ret.y, _min.y, _max.y);
    ret.z = std::clamp(ret.z, _min.z, _max.z);
    return ret;
}
Vector2 UtilityMath::Clamp(const Vector2& _target, const Vector2& _min, const Vector2& _max)
{
    // 数値の範囲制限
    Vector2 ret = _target;
    ret.x = std::clamp(ret.x, _min.x, _max.x);
    ret.y = std::clamp(ret.y, _min.y, _max.y);
    return ret;
}

int UtilityMath::Lerp(int _start, int _end, float _term)
{
    // 線形補間
    if (_term >= 1.0f)
    {
        return _end;
    }

    int ret = _start;
    ret += Round(_term * static_cast<float>(_end - _start));
    return ret;
}

float UtilityMath::Lerp(float _start, float _end, float _term)
{
    // 線形補間
    if (_term >= 1.0f)
    {
        return _end;
    }

    float ret = _start;
    ret += _term * (_end - _start);
    return ret;
}

double UtilityMath::Lerp(double _start, double _end, double _term)
{
    // 線形補間
    if (_term >= 1.0)
    {
        return _end;
    }

    double ret = _start;
    ret += _term * (_end - _start);
    return ret;
}

Vector2 UtilityMath::Lerp(const Vector2& _start, const Vector2& _end, float _term)
{
    // 線形補間
    if (_term >= 1.0f)
    {
        return _end;
    }

    Vector2 ret = _start;
    ret.x += Round(_term * static_cast<float>((_end.x - _start.x)));
    ret.y += Round(_term * static_cast<float>((_end.y - _start.y)));
    return ret;
}

VECTOR UtilityMath::Lerp(const VECTOR& _start, const VECTOR& _end, float _term)
{
    // 線形補間
    if (_term >= 1.0f)
    {
        return _end;
    }

    VECTOR ret = _start;
    ret.x += _term * (_end.x - _start.x);
    ret.y += _term * (_end.y - _start.y);
    ret.z += _term * (_end.z - _start.z);

    return ret;
}

double UtilityMath::LerpDeg(double _start, double _end, double _term)
{

    double ret;

    double diff = _end - _start;
    if (diff < -180.0)
    {
        _end += 360.0;
        ret = Lerp(_start, _end, _term);
        if (ret >= 360.0)
        {
            ret -= 360.0;
        }
    }
    else if (diff > 180.0)
    {
        _end -= 360.0;
        ret = Lerp(_start, _end, _term);
        if (ret < 0.0)
        {
            ret += 360.0;
        }
    }
    else
    {
        ret = Lerp(_start, _end, _term);
    }

    return ret;

}

COLOR_F UtilityMath::Lerp(const COLOR_F& _start, const COLOR_F& _end, float _term)
{
    // 線形補間
    if (_term >= 1.0f)
    {
        return _end;
    }

    COLOR_F ret = _start;
    ret.r += _term * (_end.r - _start.r);
    ret.g += _term * (_end.g - _start.g);
    ret.b += _term * (_end.b - _start.b);
    ret.a += _term * (_end.a - _start.a);
    return ret;
}

Vector2 UtilityMath::Bezier(const Vector2& _pos1, const Vector2& _pos2, const Vector2& _pos3, float _term)
{
    Vector2 a = Lerp(_pos1, _pos2, _term);
    Vector2 b = Lerp(_pos2, _pos3, _term);
    return Lerp(a, b, _term);
}

VECTOR UtilityMath::Bezier(const VECTOR& _pos1, const VECTOR& _pos2, const VECTOR& _pos3, float _term)
{
    VECTOR a = Lerp(_pos1, _pos2, _term);
    VECTOR b = Lerp(_pos2, _pos3, _term);
    return Lerp(a, b, _term);
}

VECTOR UtilityMath::RotXZPos(const VECTOR& _centerPos, const VECTOR& _radiusPos, float _rad)
{
    float x = ((_radiusPos.x - _centerPos.x) * cosf(_rad)) - ((_radiusPos.z - _centerPos.z) * sinf(_rad));
    float z = ((_radiusPos.x - _centerPos.x) * sinf(_rad)) + ((_radiusPos.z - _centerPos.z) * cosf(_rad));
    return VGet(_centerPos.x + x, _radiusPos.y, _centerPos.z + z);
}

double UtilityMath::Magnitude(const Vector2& _vec)
{
    return sqrt((_vec.x * _vec.x) + (_vec.y * _vec.y));
}

double UtilityMath::Magnitude(const VECTOR& _vec)
{
    return sqrt((_vec.x * _vec.x) + (_vec.y * _vec.y) + (_vec.z * _vec.z));
}

float UtilityMath::MagnitudeF(const VECTOR& _vec)
{
    return sqrtf((_vec.x * _vec.x) + (_vec.y * _vec.y) + (_vec.z * _vec.z));
}

int UtilityMath::SqrMagnitude(const Vector2& _vec)
{
    return _vec.x * _vec.x + _vec.y * _vec.y;
}

float UtilityMath::SqrMagnitudeF(const VECTOR& _vec)
{
    return _vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z;
}

double UtilityMath::SqrMagnitude(const VECTOR& _vec)
{
    return _vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z;
}

double UtilityMath::SqrMagnitude(const VECTOR& _vec1, const VECTOR& _vec2)
{
    return pow(_vec2.x - _vec1.x, 2) + pow(_vec2.y - _vec1.y, 2) + pow(_vec2.z - _vec1.z, 2);
}

double UtilityMath::Distance(const Vector2& _vec1, const Vector2& _vec2)
{
    return sqrt(pow(_vec2.x - _vec1.x, 2) + pow(_vec2.y - _vec1.y, 2));
}

double UtilityMath::Distance(const VECTOR& _vec1, const VECTOR& _vec2)
{
    return sqrt(pow(_vec2.x - _vec1.x, 2) + pow(_vec2.y - _vec1.y, 2) + pow(_vec2.z - _vec1.z, 2));
}

bool UtilityMath::IsHitPointSphere(const VECTOR& _pos1, const VECTOR& _pos2, float _radius)
{
    // 球体同士の衝突判定
    bool ret = false;

    // 座標の差からお互いの距離を取る
    VECTOR diff = VSub(_pos2, _pos1);

    // 三平方の定理で比較(SqrMagnitudeと同じ)
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < _radius)
    {
        ret = true;
    }

    return ret;
}

bool UtilityMath::IsHitSpheres(const VECTOR& _pos1, float _radius1, const VECTOR& _pos2, float _radius2)
{
    // 球体同士の衝突判定
    bool ret = false;

    // お互いの半径の合計
    float _radius = _radius1 + _radius2;

    // 座標の差からお互いの距離を取る
    VECTOR diff = VSub(_pos2, _pos1);

    // 三平方の定理で比較(SqrMagnitudeと同じ)
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < (_radius * _radius))
    {
        ret = true;
    }

    return ret;
}

bool UtilityMath::IsHitSphereCapsule(const VECTOR& _spherePos, float _sphereRadius, 
                                     const VECTOR& _capPosTop, const VECTOR& _capPosBottom, float _capRadius)
{

    bool ret = false;

    // カプセル球体の中心を繋ぐベクトル
    VECTOR cap1to2 = VSub(_capPosBottom, _capPosTop);

    // ベクトルを正規化
    VECTOR cap1to2ENor = VNorm(cap1to2);

    // カプセル繋ぎの単位ベクトルと、
    // そのベクトル元から球体へのベクトルの内積を取る
    float dot = VDot(cap1to2ENor, VSub(_spherePos, _capPosTop));

    // 内積で求めた射影距離を使って、カプセル繋ぎ上の座標を取る
    VECTOR capRidePos = VAdd(_capPosTop, VScale(cap1to2ENor, dot));

    // カプセル繋ぎのベクトルの長さを取る
    float _len = UtilityMath::MagnitudeF(cap1to2);

    // 球体がカプセル繋ぎ上にいるか判別するため、比率を取る
    float rate = dot / _len;

    VECTOR _centerPos;

    // 球体の位置が３エリアに分割されたカプセル形状のどこにいるか判別
    if (rate > 0.0f && rate <= 1.0f)
    {
        // ①球体がカプセル繋ぎ上にいる
        _centerPos = VAdd(_capPosTop, VScale(cap1to2ENor, dot));
    }
    else if (rate > 1.0f)
    {
        // ②球体がカプセルの終点側にいる
        _centerPos = _capPosBottom;
    }
    else if (rate < 0.0f)
    {
        // ③球体がカプセルの始点側にいる
        _centerPos = _capPosTop;
    }
    else
    {
        // ここにきてはいけない
    }

    // 球体同士の当たり判定
    if (UtilityMath::IsHitSpheres(_centerPos, _capRadius, _spherePos, _sphereRadius))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;

}

bool UtilityMath::Equals(const VECTOR& _vec1, const VECTOR& _vec2)
{
    if (_vec1.x == _vec2.x && _vec1.y == _vec2.y && _vec1.z == _vec2.z)
    {
        return true;
    }
    return false;
}
bool UtilityMath::Equals(const Vector2& _vec1, const Vector2& _vec2)
{
    if (_vec1.x == _vec2.x && _vec1.y == _vec2.y)
    {
        return true;
    }
    return false;
}

bool UtilityMath::EqualsVZero(const VECTOR& _vec1)
{
    const VECTOR& _vec2 = VECTOR_ZERO;
    if (_vec1.x == _vec2.x && _vec1.y == _vec2.y && _vec1.z == _vec2.z)
    {
        return true;
    }
    return false;
}
bool UtilityMath::EqualsVZero(const Vector2& _vec1)
{
    const VECTOR& _vec2 = VECTOR_ZERO;
    if (_vec1.x == _vec2.x && _vec1.y == _vec2.y)
    {
        return true;
    }
    return false;
}

bool UtilityMath::IsRange(int _num, int _min, int _max)
{
    // 範囲内にあるか否か
    return (_num >= _min && _num < _max);
}

bool UtilityMath::IsRange(float _num, float _min, float _max)
{
    // 範囲内にあるか否か
    return (_num >= _min && _num < _max);
}


VECTOR UtilityMath::Normalize(const Vector2& _vec)
{
    VECTOR ret = VGet(
        static_cast<float>(_vec.x),
        static_cast<float>(_vec.y),
        0.0f
    );
    float _len = static_cast<float>(Magnitude(_vec));
    ret.x /= _len;
    ret.y /= _len;
    ret.z /= _len;
    return ret;
}

VECTOR UtilityMath::VNormalize(const VECTOR& _vec)
{
    // Quaternion計算でゼロ除算が発生しないように判定
    if (UtilityMath::EqualsVZero(_vec))
    {
        // 元のゼロベクトルを返す
        return _vec;
    }

    // 通常正規化
    return VNorm(_vec);
}

double UtilityMath::AngleDeg(const VECTOR& _from, const VECTOR& _to)
{
    // sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
    auto fLen = SqrMagnitude(_from);
    auto tLen = SqrMagnitude(_to);
    auto denominator = sqrt(fLen * tLen);
    if (denominator < kEpsilonNormalSqrt)
    {
        return 0.0f;
    }

    //float dot = std::clamp(Dot(_from, _to) / denominator, -1.0f, 1.0f);
    //auto dot = Dot(_from, _to) / denominator;
    auto dot = VDot(_from, _to) / denominator;
    if (dot < -1.0f)
    {
        dot = -1.0f;
    }
    if (dot > 1.0f)
    {
        dot = 1.0f;
    }

    return acos(dot) * (180.0 / DX_PI);
}

void UtilityMath::DrawLineDir(const VECTOR& _pos, const VECTOR& _dir, int _color, float _len, float _sphereSize)
{
    // 球の分割数
    constexpr int SPHERE_DIV = 5;

    auto nDir = UtilityMath::VNormalize(_dir);
    auto ePos = VAdd(_pos, VScale(nDir, _len));
    DrawLine3D(_pos, ePos, _color);
    DrawSphere3D(ePos, _sphereSize, SPHERE_DIV, _color, _color, true);
}

void UtilityMath::DrawLineXYZ(const VECTOR& _pos, const Quaternion& rot, float _len)
{
    VECTOR _dir;

    // X
    _dir = rot.GetRight();
    DrawLineDir(_pos, _dir, 0xff0000, _len);

    // Y
    _dir = rot.GetUp();
    DrawLineDir(_pos, _dir, 0x00ff00, _len);

    // Z
    _dir = rot.GetForward();
    DrawLineDir(_pos, _dir, 0x0000ff, _len);

}
void UtilityMath::DrawLineXYZ(const VECTOR& _pos, const MATRIX& _rot, float _len)
{

    VECTOR _dir;

    // X
    _dir = VTransform(UtilityMath::DIR_RIGHT, _rot);
    DrawLineDir(_pos, _dir, 0xff0000, _len);

    // Y
    _dir = VTransform(UtilityMath::DIR_UP, _rot);
    DrawLineDir(_pos, _dir, 0x00ff00, _len);

    // Z
    _dir = VTransform(UtilityMath::DIR_FORWARD, _rot);
    DrawLineDir(_pos, _dir, 0x0000ff, _len);

}

MATRIX UtilityMath::GetMatrixRotateXYZ(const VECTOR& _euler)
{
    MATRIX ret = MGetIdent();
    ret = MMult(ret, MGetRotX(_euler.x));
    ret = MMult(ret, MGetRotY(_euler.y));
    ret = MMult(ret, MGetRotZ(_euler.z));
    return ret;
}

MATRIX UtilityMath::Multiplication(const MATRIX& _child, const MATRIX& _parent)
{
    return MMult(_child, _parent);
}

MATRIX UtilityMath::Multiplication(const VECTOR& _childEuler, const VECTOR& _parentEuler)
{
    // モデルの回転行列
    MATRIX mat = GetMatrixRotateXYZ(_parentEuler);

    // モデルのローカル回転行列
    MATRIX localMat = GetMatrixRotateXYZ(_childEuler);

    // 行列の合成(子, 親と指定すると親⇒子の順に適用される)
    return MMult(localMat, mat);
}

bool UtilityMath::IsHitCircleXY(
    const VECTOR& _pos1, float _radius1,
    const VECTOR& _pos2, float _radius2)
{
    float dx = _pos1.x - _pos2.x;
    float dy = _pos1.y - _pos2.y;

    float distSq = dx * dx + dy * dy;
    float radiusSum = _radius1 + _radius2;

    return distSq <= (radiusSum * radiusSum);
}