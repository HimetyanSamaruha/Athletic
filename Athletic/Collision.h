/// <summary>
/// è’ìÀÅiãÖÇ∆ãÖ
/// </summary>

#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

class Segment
{
public:
	DirectX::SimpleMath::Vector3 Start;
	DirectX::SimpleMath::Vector3 End;
};

class Capsule
{
public:

	Segment Segment;
	float Radius;
	Capsule()
	{
		Segment.Start = DirectX::SimpleMath::Vector3(0, 0, 0);
		Segment.End = DirectX::SimpleMath::Vector3(0, 1, 0);
		Radius = 1.0f;
	}
};

class Sphere
{
public:
	DirectX::SimpleMath::Vector3 Center;
	Segment segment;
	float Radius;

	Sphere()
	{
		Radius = 1.0f;
	}
};

bool CheckSpher(const Sphere & sphere1, const Sphere & sphere2);

class Triangle {
public:

	DirectX::SimpleMath::Vector3 P0;
	DirectX::SimpleMath::Vector3 P1;
	DirectX::SimpleMath::Vector3 P2;

	DirectX::SimpleMath::Vector3 Normal;

};

void ComTriangle(const DirectX::SimpleMath::Vector3& _p0, const DirectX::SimpleMath::Vector3& _p1, const DirectX::SimpleMath::Vector3& _p2, Triangle* _tri);
bool CheckSphere2Triangle(const Sphere& sphere1, const Triangle& angle, DirectX::SimpleMath::Vector3 *inter);
void ClosestPtPoint2Tri(const DirectX::SimpleMath::Vector3& sphere, const Triangle& tri, DirectX::SimpleMath::Vector3* p);
bool CheckPoint2Triangle(const DirectX::SimpleMath::Vector3& _point, const Triangle& _triangle);
bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, DirectX::SimpleMath::Vector3 *_inter);

bool Check2S(Capsule _0, Capsule _1);

float GetSqDistanceSegment2Segment(const Segment& _segment0, const Segment& _segment1);
inline float Clamp(float _x, float _min, float _max);