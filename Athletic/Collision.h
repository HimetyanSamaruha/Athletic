#pragma once
#include<d3d11.h>
#include<SimpleMath.h>

//��
class Sphere
{
public:
	//���̒��S���W
	DirectX::SimpleMath::Vector3 Center;

	//���a
	float Radius;

	Sphere() {
		//�f�t�H���g�̔��a��1���[�g����
		Radius = 1.0f;
	}
};

// ��
class Box {
public:
	// ���_���W
	DirectX::SimpleMath::Vector3 Pos0;// ����O
	DirectX::SimpleMath::Vector3 Pos1;// �E��O
	DirectX::SimpleMath::Vector3 Pos2;// �����O
	DirectX::SimpleMath::Vector3 Pos3;// �E���O

	DirectX::SimpleMath::Vector3 Pos4;// �����
	DirectX::SimpleMath::Vector3 Pos5;// �E���
	DirectX::SimpleMath::Vector3 Pos6;// ������
	DirectX::SimpleMath::Vector3 Pos7;// �E����

};

//����
class Segment {
public:
	DirectX::SimpleMath::Vector3 Start;
	DirectX::SimpleMath::Vector3 End;

};

class Triangle {
public:
	//	���_���W
	DirectX::SimpleMath::Vector3 P0;
	DirectX::SimpleMath::Vector3 P1;
	DirectX::SimpleMath::Vector3 P2;
	// �@���x�N�g��
	DirectX::SimpleMath::Vector3 Normal;
};


bool CheckSphere2Sphere(const Sphere& _sphereA, const Sphere& _sphereB);

void ComputeTriangle(const DirectX::SimpleMath::Vector3 & _p0, const DirectX::SimpleMath::Vector3 & _p1, const DirectX::SimpleMath::Vector3 & _p2, Triangle * _triangle);

bool CheckSegment2Triangle(const Segment & _segment, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _inter);

bool CheckSphere2Triangle(const Sphere & _sphere, Triangle & _triangle, DirectX::SimpleMath::Vector3* _inter);

bool CheckSphere2Box(const Sphere& _sphere, const Box& _box, DirectX::SimpleMath::Vector3* _inter);

bool CheckBox2BoxAABB(Box _box1, Box _box2, DirectX::SimpleMath::Vector3* _inter);