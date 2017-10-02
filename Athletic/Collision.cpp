#include "Collision.h"

using namespace std;
using namespace DirectX::SimpleMath;

/// <summary>
/// 球と球のあたり判定
/// </summary>
/// <param name="sphere1">球１</param>
/// <param name="sphere2">球２</param>
/// <returns>true:ヒット　false:ヒットしない</returns>
bool CheckSpher(const Sphere& sphere1, const Sphere& sphere2)
{
	//球１と球２の中心座標を調べる
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

//元ComputeTriangle
void ComTriangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, Triangle* _tri)
{
	//座標のコピー
	_tri->P0 = _p0;
	_tri->P1 = _p1;
	_tri->P2 = _p2;

	//法線ベクトル
	Vector3 P0_P1 = _tri->P1 - _tri->P0;
	Vector3 P1_P2 = _tri->P2 - _tri->P1;

	_tri->Normal = P0_P1.Cross(P1_P2);
	_tri->Normal.Normalize();
}

/// <summary>
/// 球と法線付き三角形の最近接点
/// </summary>
/// <param name="sphere1">点</param>
/// <param name="angle">三角形</param>
/// <param name="inter">最近接点</param>
/// <returns></returns>
void ClosestPtPoint2Tri(const Vector3& point, const Triangle& tri, Vector3* p)
{
	//point P0の外側の頂点領域の中にあるのか
	Vector3 P0_P1 = tri.P1 - tri.P0;
	Vector3 P0_P2 = tri.P2 - tri.P0;
	Vector3 P0_PI = point - tri.P0;

	float d1 = P0_P1.Dot(P0_PI);
	float d2 = P0_P2.Dot(P0_PI);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		//P0が一番近い
		*p = tri.P0;
		return;
	}

	//pointがP1の外側の頂点領域にあるかチェック
	Vector3 P1_PT = point - tri.P1;

	float d3 = P0_P1.Dot(P1_PT);
	float d4 = P0_P2.Dot(P1_PT);

	if (d3 > 0.0f && d4 > 0.0f)
	{
		//P1が一番近い
		*p = tri.P1;
		return;
	}

	//pointがP0_P1の辺領域の中にあるかどうか
	//あればpointのP0_P1上に対する射影を返す
	float vc = d1*d4 - d3*d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*p = tri.P0 + v * P0_P1;
		return;
	}

	// _pointがP2の外側の頂点領域の中にあるかどうかチェック
	Vector3 P2_PT = point - tri.P2;

	float d5 = P0_P1.Dot(P2_PT);
	float d6 = P0_P2.Dot(P2_PT);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*p = tri.P2;
		return;
	}

	// _pointがP0_P2の辺領域の中にあるかどうかチェックし、あれば_pointのP0_P2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*p = tri.P0 + w * P0_P2;
		return;
	}

	// _pointがP1_P2の辺領域の中にあるかどうかチェックし、あれば_pointのP1_P2上に対する射影を返す
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
	//点と三角形は同一平面上にあるものとしています。同一平面上に無い場合は正しい結果になりません
	//線上は外とみなします。
	//ABCが三角形かどうかのチェックは省略...

	// 点0→1、 1→2、 2→0 のベクトルをそれぞれ計算
	Vector3 v01 = _triangle.P1 - _triangle.P0;
	Vector3 v12 = _triangle.P2 - _triangle.P1;
	Vector3 v20 = _triangle.P0 - _triangle.P2;
	// 三角形の各頂点から点へのベクトルをそれぞれ計算
	Vector3 v0p = _point - _triangle.P0;
	Vector3 v1p = _point - _triangle.P1;
	Vector3 v2p = _point - _triangle.P2;
	// 各辺ベクトルと、点へのベクトルの外積を計算
	Vector3 c0 = v01.Cross(v0p);
	Vector3 c1 = v12.Cross(v1p);
	Vector3 c2 = v20.Cross(v2p);
	// 内積で同じ方向かどうか調べる
	float dot01 = c0.Dot(c1);
	float dot02 = c0.Dot(c2);
	// 外積ベクトルが全て同じ方向なら、三角形の内側に点がある
	if (dot01 > 0 && dot02 > 0)
	{
		return true;
	}

	//三角形の外側に点がある
	return false;

}

/// <summary>
/// 球と法線付き三角形のあたり判定
/// </summary>
/// <param name="sphere1">球</param>
/// <param name="angle">三角形</param>
/// <param name="inter">交点座標</param>
/// <returns></returns>
bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& tri, Vector3 *inter)
{
	Vector3 p;

	//球の中心に一番近い点である三角形上の点Ｐをみつける
	ClosestPtPoint2Tri(sphere.Center, tri, &p);

	Vector3 v = p - sphere.Center;

	// 球と三角形が交差するのは、球の中心から点pまでの距離が球の半径よりも小さい場合
	if (v.Dot(v) <= sphere.Radius * sphere.Radius)
	{
		if (inter)
		{
			*inter = p;
		}

		return true;
	}

	return false;

	// 球と平面（三角形が乗っている平面）の当たり判定
	// 球と平面の距離を計算
	float ds = sphere.Center.Dot(tri.Normal);
	float dt = tri.P0.Dot(tri.Normal);
	float dist = ds - dt;
	// 距離が半径以上なら、当たらない
	if (fabsf(dist) > sphere.Radius)	return false;
	// 中心点を平面に射影したとき、三角形の内側にあれば、当たっている
	// 射影した座標
	Vector3 center = -dist * tri.Normal + sphere.Center;

	// 三角形の外側になければ、当たっていない
	if (!CheckPoint2Triangle(center, tri))	return false;

	if (inter)
	{
		*inter = center;	// 交点をコピー
	}

	return true;
	
}

//カプセルとの判定
bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, Vector3 *_inter)
{
	const float epsilon = -1.0e-5f;	// 誤差吸収用の微小な値
	Vector3 	LayV;		// 線分の終点→始点
	Vector3 	tls;		// 三角形の頂点0→線分の始点
	Vector3 	tle;		// 三角形の頂点0→線分の終点
	float 	distl0;
	float 	distl1;
	float 	dp;
	float 	denom;
	float 	t;
	Vector3	s;			// 直線と平面との交点
	Vector3 	st0;		// 交点→三角形の頂点0
	Vector3 	st1;		// 交点→三角形の頂点1
	Vector3 	st2;		// 交点→三角形の頂点2
	Vector3 	t01;		// 三角形の頂点0→頂点1
	Vector3 	t12;		// 三角形の頂点1→頂点2
	Vector3 	t20;		// 三角形の頂点2→頂点0
	Vector3	m;

	// 線分の始点が三角系の裏側にあれば、当たらない
	tls = _segment.Start - _triangle.P0;
	distl0 = tls.Dot(_triangle.Normal);	// 線分の始点と平面の距離
	if (distl0 <= epsilon) return false;

	// 線分の終点が三角系の表側にあれば、当たらない
	tle = _segment.End - _triangle.P0;
	distl1 = tle.Dot(_triangle.Normal);	// 線分の終点と平面の距離
	if (distl1 >= -epsilon) return false;

	// 直線と平面との交点sを取る
	denom = distl0 - distl1;
	t = distl0 / denom;
	LayV = _segment.End - _segment.Start;	// 線分の方向ベクトルを取得
	s = t * LayV + _segment.Start;

	// 交点が三角形の内側にあるかどうかを調べる。
	// 三角形の内側にある場合、交点から各頂点へのベクトルと各辺ベクトルの外積（三組）が、全て法線と同じ方向を向く
	// 一つでも方向が一致しなければ、当たらない。
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
		*_inter = s;	// 交点をコピー
	}

	return true;
}

//カプセル同士のあたり判定
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
	const float epsilon = 1.0e-5f;	// 誤差吸収用の微小な値
	Vector3 SegmentSub;
	Vector3 SegmentPoint;
	Vector3 CP;

	// 線分の始点から終点へのベクトル
	SegmentSub = _segment.End - _segment.Start;

	// 線分の始点から点へのベクトル
	SegmentPoint = _point - _segment.Start;
	if (SegmentSub.Dot(SegmentPoint) < epsilon)
	{// ２ベクトルの内積が負なら、線分の始点が最近傍
		return SegmentPoint.Dot(SegmentPoint);
	}

	// 点から線分の終点へのベクトル
	SegmentPoint = _segment.End - _point;
	if (SegmentSub.Dot(SegmentPoint) < epsilon)
	{// ２ベクトルの内積が負なら、線分の終点が最近傍
		return SegmentPoint.Dot(SegmentPoint);
	}

	// 上記のどちらにも該当しない場合、線分上に落とした射影が最近傍
	// (本来ならサインで求めるが、外積の大きさ/線分のベクトルの大きさで求まる)
	CP.Cross(SegmentSub, SegmentPoint);

	return CP.Dot(CP) / SegmentSub.Dot(SegmentSub);
}

float GetSqDistanceSegment2Segment(const Segment& _segment0, const Segment& _segment1)
{
	const float epsilon = 1.0e-5f;	// 誤差吸収用の微小な値
	Vector3 d0, d1, r;
	Vector3 c0, c1;	// 二つの線分上の最接近点
	Vector3 v;		// c1→c0ベクトル
	float a, b, c, e, f;
	float s, t;
	float denom;
	float tnom;

	d0 = _segment0.End - _segment0.Start;	// 線分0の方向ベクトル
	d1 = _segment1.End - _segment1.Start; // 線分1の方向ベクトル
	r = _segment0.Start - _segment1.Start; // 線分1の始点から線分0の始点へのベクトル
	a = d0.Dot(d0);		// 線分0の距離の二乗
	e = d1.Dot(d1);		// 線分1の距離の二乗
	//	b = d0.dot(d1);		// 最適化の為後方に移動した
	//	c = d0.dot(r);		// 最適化の為後方に移動した
	//	f = d1.dot(r);		// 最適化の為後方に移動した

						// いづれかの線分の長さが0かどうかチェック
	if (a <= epsilon && e <= epsilon)
	{// 両方長さ0
		v = _segment0.Start - _segment1.Start;

		return v.Dot(v);
	}

	if (a <= epsilon)
	{// 線分0が長さ0
		return GetSqDistancePoint2Segment(_segment0.Start, _segment1);
	}

	if (e <= epsilon)
	{// 線分1が長さ0
		return GetSqDistancePoint2Segment(_segment1.Start, _segment0);
	}

	b = d0.Dot(d1);
	f = d1.Dot(r);
	c = d0.Dot(r);

	denom = a * e - b * b;	// 常に非負
							// 線分が平行でない場合、直線0上の直線1に対する最近接点を計算、そして
							// 線分0上にクランプ。そうでない場合は任意のsを選択
	if (denom != 0)
	{
		s = Clamp((b * f - c * e) / denom, 0, 1);
	}
	else
	{
		s = 0;
	}

	// 直線1上の最接近点を計算
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

// 最小値と最大値の間にクランプする
inline float Clamp(float _x, float _min, float _max)
{
	return min(max(_x, _min), _max);
}

bool CheckSphere2Box(const Sphere& _sphere, const Box& _box, Vector3* _inter)
{
	Vector3 p;

	Triangle boxTriangle[12];

	ComTriangle(_box.Pos0, _box.Pos1, _box.Pos2, &boxTriangle[0]);
	ComTriangle(_box.Pos1, _box.Pos2, _box.Pos3, &boxTriangle[1]);
	ComTriangle(_box.Pos1, _box.Pos4, _box.Pos3, &boxTriangle[2]);
	ComTriangle(_box.Pos4, _box.Pos3, _box.Pos6, &boxTriangle[3]);
	ComTriangle(_box.Pos4, _box.Pos5, _box.Pos6, &boxTriangle[4]);
	ComTriangle(_box.Pos5, _box.Pos6, _box.Pos7, &boxTriangle[5]);
	ComTriangle(_box.Pos5, _box.Pos0, _box.Pos7, &boxTriangle[6]);
	ComTriangle(_box.Pos0, _box.Pos7, _box.Pos2, &boxTriangle[7]);
	ComTriangle(_box.Pos5, _box.Pos4, _box.Pos0, &boxTriangle[8]);
	ComTriangle(_box.Pos4, _box.Pos0, _box.Pos1, &boxTriangle[9]);
	ComTriangle(_box.Pos2, _box.Pos3, _box.Pos7, &boxTriangle[10]);
	ComTriangle(_box.Pos3, _box.Pos7, _box.Pos6, &boxTriangle[11]);

	if (
		CheckSphere2Triangle(_sphere, boxTriangle[0], &p) ||
		CheckSphere2Triangle(_sphere, boxTriangle[1], &p) ||
		CheckSphere2Triangle(_sphere, boxTriangle[2], &p) ||
		CheckSphere2Triangle(_sphere, boxTriangle[3], &p) ||
		CheckSphere2Triangle(_sphere, boxTriangle[4], &p) ||
		CheckSphere2Triangle(_sphere, boxTriangle[5], &p) ||
		CheckSphere2Triangle(_sphere, boxTriangle[6], &p) ||
		CheckSphere2Triangle(_sphere, boxTriangle[7], &p) ||
		CheckSphere2Triangle(_sphere, boxTriangle[8], &p) ||
		CheckSphere2Triangle(_sphere, boxTriangle[9], &p) ||
		CheckSphere2Triangle(_sphere, boxTriangle[10], &p) ||
		CheckSphere2Triangle(_sphere, boxTriangle[11], &p)
		)
	{
		if (_inter)
		{
			*_inter = p;
		}

		return true;
	}

	return false;

}

bool CheckCapsuleSphere2Box(const Capsule& _Capsule, const Box& _box, Vector3* _inter)
{
	Vector3 p;

	Triangle boxTriangle[12];

	ComTriangle(_box.Pos0, _box.Pos1, _box.Pos2, &boxTriangle[0]);
	ComTriangle(_box.Pos1, _box.Pos2, _box.Pos3, &boxTriangle[1]);
	ComTriangle(_box.Pos1, _box.Pos4, _box.Pos3, &boxTriangle[2]);
	ComTriangle(_box.Pos4, _box.Pos3, _box.Pos6, &boxTriangle[3]);
	ComTriangle(_box.Pos4, _box.Pos5, _box.Pos6, &boxTriangle[4]);
	ComTriangle(_box.Pos5, _box.Pos6, _box.Pos7, &boxTriangle[5]);
	ComTriangle(_box.Pos5, _box.Pos0, _box.Pos7, &boxTriangle[6]);
	ComTriangle(_box.Pos0, _box.Pos7, _box.Pos2, &boxTriangle[7]);
	ComTriangle(_box.Pos5, _box.Pos4, _box.Pos0, &boxTriangle[8]);
	ComTriangle(_box.Pos4, _box.Pos0, _box.Pos1, &boxTriangle[9]);
	ComTriangle(_box.Pos2, _box.Pos3, _box.Pos7, &boxTriangle[10]);
	ComTriangle(_box.Pos3, _box.Pos7, _box.Pos6, &boxTriangle[11]);

	Sphere CapsuleSphere;

	CapsuleSphere.Center;

	float h = _Capsule.Segment.Start.y - _Capsule.Segment.End.y;

	if (h < 0)
	{
		h = h*-1;
	}

	for (int i = 0; i < h; i++)
	{
		CapsuleSphere.Center = Vector3(_Capsule.Segment.End.x, _Capsule.Segment.End.y + h, _Capsule.Segment.End.z);

		if (CheckSphere2Triangle(CapsuleSphere, boxTriangle[0], &p) ||
			CheckSphere2Triangle(CapsuleSphere, boxTriangle[1], &p) ||
			CheckSphere2Triangle(CapsuleSphere, boxTriangle[2], &p) ||
			CheckSphere2Triangle(CapsuleSphere, boxTriangle[3], &p) ||
			CheckSphere2Triangle(CapsuleSphere, boxTriangle[4], &p) ||
			CheckSphere2Triangle(CapsuleSphere, boxTriangle[5], &p) ||
			CheckSphere2Triangle(CapsuleSphere, boxTriangle[6], &p) ||
			CheckSphere2Triangle(CapsuleSphere, boxTriangle[7], &p) ||
			CheckSphere2Triangle(CapsuleSphere, boxTriangle[8], &p) ||
			CheckSphere2Triangle(CapsuleSphere, boxTriangle[9], &p) ||
			CheckSphere2Triangle(CapsuleSphere, boxTriangle[10], &p) ||
			CheckSphere2Triangle(CapsuleSphere, boxTriangle[11], &p))
		{

			if (_inter)
			{
				*_inter = p;
			}

			return true;
		}
	}

	return false;
}