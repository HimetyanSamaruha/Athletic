#include "Collision.h"

using namespace std;
using namespace DirectX::SimpleMath;

/// <summary>
/// ���Ƌ��̂����蔻��
/// </summary>
/// <param name="sphere1">���P</param>
/// <param name="sphere2">���Q</param>
/// <returns>true:�q�b�g�@false:�q�b�g���Ȃ�</returns>
bool CheckSpher(const Sphere& sphere1, const Sphere& sphere2)
{
	//���P�Ƌ��Q�̒��S���W�𒲂ׂ�
	Vector3 sub = sphere1.Center - sphere2.Center;

	float dis = sqrtf(sub.x *sub.x + sub.y * sub.y + sub.z*sub.z);

	float radSp;

	radSp = sphere1.Radius + sphere2.Radius;

	radSp = radSp * radSp;

	if (dis > radSp)
	{
		return false;
	}
	return true;
}

void ComTriangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, Triangle* _tri)
{
	//���W�̃R�s�[
	_tri->P0 = _p0;
	_tri->P1 = _p1;
	_tri->P2 = _p2;

	//�@���x�N�g��
	Vector3 P0_P1 = _tri->P1 - _tri->P0;
	Vector3 P1_P2 = _tri->P2 - _tri->P1;

	_tri->Normal = P0_P1.Cross(P1_P2);
	_tri->Normal.Normalize();
}

/// <summary>
/// ���Ɩ@���t���O�p�`�̍ŋߐړ_
/// </summary>
/// <param name="sphere1">�_</param>
/// <param name="angle">�O�p�`</param>
/// <param name="inter">�ŋߐړ_</param>
/// <returns></returns>
void ClosestPtPoint2Tri(const Vector3& point, const Triangle& tri, Vector3* p)
{
	//point P0�̊O���̒��_�̈�̒��ɂ���̂�
	Vector3 P0_P1 = tri.P1 - tri.P0;
	Vector3 P0_P2 = tri.P2 - tri.P0;
	Vector3 P0_PI = point - tri.P0;

	float d1 = P0_P1.Dot(P0_PI);
	float d2 = P0_P2.Dot(P0_PI);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		//P0����ԋ߂�
		*p = tri.P0;
		return;
	}

	//point��P1�̊O���̒��_�̈�ɂ��邩�`�F�b�N
	Vector3 P1_PT = point - tri.P1;

	float d3 = P0_P1.Dot(P1_PT);
	float d4 = P0_P2.Dot(P1_PT);

	if (d3 > 0.0f && d4 > 0.0f)
	{
		//P1����ԋ߂�
		*p = tri.P1;
		return;
	}

	//point��P0_P1�̕ӗ̈�̒��ɂ��邩�ǂ���
	//�����point��P0_P1��ɑ΂���ˉe��Ԃ�
	float vc = d1*d4 - d3*d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*p = tri.P0 + v * P0_P1;
		return;
	}

	// _point��P2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 P2_PT = point - tri.P2;

	float d5 = P0_P1.Dot(P2_PT);
	float d6 = P0_P2.Dot(P2_PT);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*p = tri.P2;
		return;
	}

	// _point��P0_P2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����_point��P0_P2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*p = tri.P0 + w * P0_P2;
		return;
	}

	// _point��P1_P2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����_point��P1_P2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*p = tri.P1 + w * (tri.P2 - tri.P1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*p = tri.P0 + P0_P1 * v + P0_P2 * w;

}


bool CheckPoint2Triangle(const Vector3& _point, const Triangle& _triangle)
{
	//�_�ƎO�p�`�͓��ꕽ�ʏ�ɂ�����̂Ƃ��Ă��܂��B���ꕽ�ʏ�ɖ����ꍇ�͐��������ʂɂȂ�܂���
	//����͊O�Ƃ݂Ȃ��܂��B
	//ABC���O�p�`���ǂ����̃`�F�b�N�͏ȗ�...

	// �_0��1�A 1��2�A 2��0 �̃x�N�g�������ꂼ��v�Z
	Vector3 v01 = _triangle.P1 - _triangle.P0;
	Vector3 v12 = _triangle.P2 - _triangle.P1;
	Vector3 v20 = _triangle.P0 - _triangle.P2;
	// �O�p�`�̊e���_����_�ւ̃x�N�g�������ꂼ��v�Z
	Vector3 v0p = _point - _triangle.P0;
	Vector3 v1p = _point - _triangle.P1;
	Vector3 v2p = _point - _triangle.P2;
	// �e�Ӄx�N�g���ƁA�_�ւ̃x�N�g���̊O�ς��v�Z
	Vector3 c0 = v01.Cross(v0p);
	Vector3 c1 = v12.Cross(v1p);
	Vector3 c2 = v20.Cross(v2p);
	// ���ςœ����������ǂ������ׂ�
	float dot01 = c0.Dot(c1);
	float dot02 = c0.Dot(c2);
	// �O�σx�N�g�����S�ē��������Ȃ�A�O�p�`�̓����ɓ_������
	if (dot01 > 0 && dot02 > 0)
	{
		return true;
	}

	//�O�p�`�̊O���ɓ_������
	return false;

}

/// <summary>
/// ���Ɩ@���t���O�p�`�̂����蔻��
/// </summary>
/// <param name="sphere1">��</param>
/// <param name="angle">�O�p�`</param>
/// <param name="inter">��_���W</param>
/// <returns></returns>
bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& tri, Vector3 *inter)
{
	Vector3 p;

	//���̒��S�Ɉ�ԋ߂��_�ł���O�p�`��̓_�o���݂���
	ClosestPtPoint2Tri(sphere.Center, tri, &p);

	Vector3 v = p - sphere.Center;

	// ���ƎO�p�`����������̂́A���̒��S����_p�܂ł̋��������̔��a�����������ꍇ
	if (v.Dot(v) <= sphere.Radius * sphere.Radius)
	{
		if (inter)
		{
			*inter = p;
		}

		return true;
	}

	return false;

	// ���ƕ��ʁi�O�p�`������Ă��镽�ʁj�̓����蔻��
	// ���ƕ��ʂ̋������v�Z
	float ds = sphere.Center.Dot(tri.Normal);
	float dt = tri.P0.Dot(tri.Normal);
	float dist = ds - dt;
	// ���������a�ȏ�Ȃ�A������Ȃ�
	if (fabsf(dist) > sphere.Radius)	return false;
	// ���S�_�𕽖ʂɎˉe�����Ƃ��A�O�p�`�̓����ɂ���΁A�������Ă���
	// �ˉe�������W
	Vector3 center = -dist * tri.Normal + sphere.Center;

	// �O�p�`�̊O���ɂȂ���΁A�������Ă��Ȃ�
	if (!CheckPoint2Triangle(center, tri))	return false;

	if (inter)
	{
		*inter = center;	// ��_���R�s�[
	}

	return true;
	
}

bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, Vector3 *_inter)
{
	const float epsilon = -1.0e-5f;	// �덷�z���p�̔����Ȓl
	Vector3 	LayV;		// �����̏I�_���n�_
	Vector3 	tls;		// �O�p�`�̒��_0�������̎n�_
	Vector3 	tle;		// �O�p�`�̒��_0�������̏I�_
	float 	distl0;
	float 	distl1;
	float 	dp;
	float 	denom;
	float 	t;
	Vector3	s;			// �����ƕ��ʂƂ̌�_
	Vector3 	st0;		// ��_���O�p�`�̒��_0
	Vector3 	st1;		// ��_���O�p�`�̒��_1
	Vector3 	st2;		// ��_���O�p�`�̒��_2
	Vector3 	t01;		// �O�p�`�̒��_0�����_1
	Vector3 	t12;		// �O�p�`�̒��_1�����_2
	Vector3 	t20;		// �O�p�`�̒��_2�����_0
	Vector3	m;

	// �����̎n�_���O�p�n�̗����ɂ���΁A������Ȃ�
	tls = _segment.Start - _triangle.P0;
	distl0 = tls.Dot(_triangle.Normal);	// �����̎n�_�ƕ��ʂ̋���
	if (distl0 <= epsilon) return false;

	// �����̏I�_���O�p�n�̕\���ɂ���΁A������Ȃ�
	tle = _segment.End - _triangle.P0;
	distl1 = tle.Dot(_triangle.Normal);	// �����̏I�_�ƕ��ʂ̋���
	if (distl1 >= -epsilon) return false;

	// �����ƕ��ʂƂ̌�_s�����
	denom = distl0 - distl1;
	t = distl0 / denom;
	LayV = _segment.End - _segment.Start;	// �����̕����x�N�g�����擾
	s = t * LayV + _segment.Start;

	// ��_���O�p�`�̓����ɂ��邩�ǂ����𒲂ׂ�B
	// �O�p�`�̓����ɂ���ꍇ�A��_����e���_�ւ̃x�N�g���Ɗe�Ӄx�N�g���̊O�ρi�O�g�j���A�S�Ė@���Ɠ�������������
	// ��ł���������v���Ȃ���΁A������Ȃ��B
	st0 = _triangle.P0 - s;
	t01 = _triangle.P1 - _triangle.P0;
	m = st0.Cross(t01);
	dp = m.Dot(_triangle.Normal);
	if (dp <= epsilon) return false;

	st1 = _triangle.P1 - s;
	t12 = _triangle.P2 - _triangle.P1;
	m = st1.Cross(t12);
	dp = m.Dot(_triangle.Normal);
	if (dp <= epsilon) return false;

	st2 = _triangle.P2 - s;
	t20 = _triangle.P0 - _triangle.P2;
	m = st2.Cross(t20);
	dp = m.Dot(_triangle.Normal);
	if (dp <= epsilon) return false;

	if (_inter)
	{
		*_inter = s;	// ��_���R�s�[
	}

	return true;
}

bool Check2S(Capsule _0, Capsule _1)
{
	float disSQ = GetSqDistanceSegment2Segment(_0.Segment,_1.Segment);

	float radiusSum = _0.Radius + _1.Radius;
	float radSQ = radiusSum * radiusSum;

	if (disSQ > radSQ) return false;

	return true;
}

float GetSqDistancePoint2Segment(const Vector3& _point, const Segment& _segment)
{
	const float epsilon = 1.0e-5f;	// �덷�z���p�̔����Ȓl
	Vector3 SegmentSub;
	Vector3 SegmentPoint;
	Vector3 CP;

	// �����̎n�_����I�_�ւ̃x�N�g��
	SegmentSub = _segment.End - _segment.Start;

	// �����̎n�_����_�ւ̃x�N�g��
	SegmentPoint = _point - _segment.Start;
	if (SegmentSub.Dot(SegmentPoint) < epsilon)
	{// �Q�x�N�g���̓��ς����Ȃ�A�����̎n�_���ŋߖT
		return SegmentPoint.Dot(SegmentPoint);
	}

	// �_��������̏I�_�ւ̃x�N�g��
	SegmentPoint = _segment.End - _point;
	if (SegmentSub.Dot(SegmentPoint) < epsilon)
	{// �Q�x�N�g���̓��ς����Ȃ�A�����̏I�_���ŋߖT
		return SegmentPoint.Dot(SegmentPoint);
	}

	// ��L�̂ǂ���ɂ��Y�����Ȃ��ꍇ�A������ɗ��Ƃ����ˉe���ŋߖT
	// (�{���Ȃ�T�C���ŋ��߂邪�A�O�ς̑傫��/�����̃x�N�g���̑傫���ŋ��܂�)
	CP.Cross(SegmentSub, SegmentPoint);

	return CP.Dot(CP) / SegmentSub.Dot(SegmentSub);
}

float GetSqDistanceSegment2Segment(const Segment& _segment0, const Segment& _segment1)
{
	const float epsilon = 1.0e-5f;	// �덷�z���p�̔����Ȓl
	Vector3 d0, d1, r;
	Vector3 c0, c1;	// ��̐�����̍Őڋߓ_
	Vector3 v;		// c1��c0�x�N�g��
	float a, b, c, e, f;
	float s, t;
	float denom;
	float tnom;

	d0 = _segment0.End - _segment0.Start;	// ����0�̕����x�N�g��
	d1 = _segment1.End - _segment1.Start; // ����1�̕����x�N�g��
	r = _segment0.Start - _segment1.Start; // ����1�̎n�_�������0�̎n�_�ւ̃x�N�g��
	a = d0.Dot(d0);		// ����0�̋����̓��
	e = d1.Dot(d1);		// ����1�̋����̓��
	//	b = d0.dot(d1);		// �œK���̈׌���Ɉړ�����
	//	c = d0.dot(r);		// �œK���̈׌���Ɉړ�����
	//	f = d1.dot(r);		// �œK���̈׌���Ɉړ�����

						// ���Âꂩ�̐����̒�����0���ǂ����`�F�b�N
	if (a <= epsilon && e <= epsilon)
	{// ��������0
		v = _segment0.Start - _segment1.Start;

		return v.Dot(v);
	}

	if (a <= epsilon)
	{// ����0������0
		return GetSqDistancePoint2Segment(_segment0.Start, _segment1);
	}

	if (e <= epsilon)
	{// ����1������0
		return GetSqDistancePoint2Segment(_segment1.Start, _segment0);
	}

	b = d0.Dot(d1);
	f = d1.Dot(r);
	c = d0.Dot(r);

	denom = a * e - b * b;	// ��ɔ�
							// ���������s�łȂ��ꍇ�A����0��̒���1�ɑ΂���ŋߐړ_���v�Z�A������
							// ����0��ɃN�����v�B�����łȂ��ꍇ�͔C�ӂ�s��I��
	if (denom != 0)
	{
		s = Clamp((b * f - c * e) / denom, 0, 1);
	}
	else
	{
		s = 0;
	}

	// ����1��̍Őڋߓ_���v�Z
	tnom = b * s + f;

	if (tnom < 0)
	{
		t = 0;
		s = Clamp(-c / a, 0, 1);
	}
	else if (tnom > e)
	{
		t = 1;
		s = Clamp((b - c) / a, 0, 1);
	}
	else
	{
		t = tnom / e;
	}

	c0 = s * d0 + _segment0.Start;
	c1 = t * d1 + _segment1.Start;
	v = c0 - c1;

	return v.Dot(v);
}

// �ŏ��l�ƍő�l�̊ԂɃN�����v����
inline float Clamp(float _x, float _min, float _max)
{
	return min(max(_x, _min), _max);
}