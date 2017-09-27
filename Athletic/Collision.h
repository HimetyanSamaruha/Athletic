#pragma once
#include<d3d11.h>
#include<SimpleMath.h>

//球
class Sphere
{
public:
	//球の中心座標
	DirectX::SimpleMath::Vector3 Center;

	//半径
	float Radius;

	Sphere() {
		//デフォルトの半径を1メートルに
		Radius = 1.0f;
	}
};

// 箱
class Box {
public:
	// 頂点座標
	DirectX::SimpleMath::Vector3 Pos0;
	DirectX::SimpleMath::Vector3 Pos1;
	DirectX::SimpleMath::Vector3 Pos2;
	DirectX::SimpleMath::Vector3 Pos3;
	DirectX::SimpleMath::Vector3 Pos4;
	DirectX::SimpleMath::Vector3 Pos5;
	DirectX::SimpleMath::Vector3 Pos6;
	DirectX::SimpleMath::Vector3 Pos7;



	//// 大きさ(縦,横,高さ)
	//DirectX::SimpleMath::Vector3 Size;

	//Box() {
	//	// デフォルトの大きさを1に
	//	Size = Vector3(1, 1, 1);
	//}
};

//線分
class Segment {
public:
	DirectX::SimpleMath::Vector3 Start;
	DirectX::SimpleMath::Vector3 End;

};

class Triangle {
public:
	//	頂点座標
	DirectX::SimpleMath::Vector3 P0;
	DirectX::SimpleMath::Vector3 P1;
	DirectX::SimpleMath::Vector3 P2;
	// 法線ベクトル
	DirectX::SimpleMath::Vector3 Normal;
};


bool CheckSphere2Sphere(const Sphere& _sphereA, const Sphere& _sphereB);

void ComputeTriangle(const DirectX::SimpleMath::Vector3 & _p0, const DirectX::SimpleMath::Vector3 & _p1, const DirectX::SimpleMath::Vector3 & _p2, Triangle * _triangle);

bool CheckSegment2Triangle(const Segment & _segment, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _inter);

bool CheckSphere2Triangle(const Sphere & _sphere, Triangle & _triangle, DirectX::SimpleMath::Vector3* _inter);

bool CheckSphere2Box(const Sphere& _sphere, const Box& _box, DirectX::SimpleMath::Vector3* _inter);
