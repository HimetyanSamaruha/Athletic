#include "CollisionNode.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

bool CollisionNode::m_DebugVisible = false;


void CollisionNode::SetParent(Obj3d * parent)
{
	m_Obj.Set_perant(parent);
}

SphereNode::SphereNode()
{
	// デフォルトの半径１メートル
	m_localRadius = 1.0f;
}

void SphereNode::Initialize()
{
	m_Obj.LoadModel(L"Resource/SphereNode.cmo");
}

void SphereNode::Update()
{
	m_Obj.Set_trans(m_Trans);
	m_Obj.Set_scale(Vector3(m_localRadius));

	m_Obj.Update();

	{// 判定球の要素を計算
		const Matrix& worldm = m_Obj.Get_world();

		// モデル座標系での中心点
		Vector3 center(0, 0, 0);
		// モデル座標系での右端の点
		Vector3 right(1, 0, 0);

		// ワールド座標系に変換
		center = Vector3::Transform(center, worldm);
		right = Vector3::Transform(right, worldm);

		// 判定球の要素を代入
		Sphere::Center = center;
		Sphere::Radius = Vector3::Distance(center, right);
	}
}

void SphereNode::Render()
{
	m_Obj.Set_trans(m_Trans);
	m_Obj.Set_scale(Vector3(m_localRadius));

	m_Obj.Draw();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//                          BoxNode　　　　　　　　　　　　　 //
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

BoxNode::BoxNode() 
{
	m_size = Vector3(1.0f, 1.0f, 1.0f);
}

void BoxNode::Initialize() 
{
	m_Obj.LoadModel(L"Resource/boxNode.cmo");

}

void BoxNode::Update()
{
	m_Obj.Set_trans(m_Trans);
	SetPointPos();

	m_Obj.Update();
}

void BoxNode::Render()
{
	m_Obj.Set_trans(m_Trans);

	m_Obj.Draw();
}

void BoxNode::SetPointPos()
{
	Pos0 = Vector3(m_Trans.x - (m_size.x / 2.0f), m_Trans.y + (m_size.y / 2.0f), m_Trans.z - (m_size.z / 2.0f));
	Pos1 = Vector3(m_Trans.x + (m_size.x / 2.0f), m_Trans.y + (m_size.y / 2.0f), m_Trans.z - (m_size.z / 2.0f));
	Pos2 = Vector3(m_Trans.x - (m_size.x / 2.0f), m_Trans.y - (m_size.y / 2.0f), m_Trans.z - (m_size.z / 2.0f));
	Pos3 = Vector3(m_Trans.x + (m_size.x / 2.0f), m_Trans.y - (m_size.y / 2.0f), m_Trans.z - (m_size.z / 2.0f));
	Pos4 = Vector3(m_Trans.x - (m_size.x / 2.0f), m_Trans.y + (m_size.y / 2.0f), m_Trans.z + (m_size.z / 2.0f));
	Pos5 = Vector3(m_Trans.x + (m_size.x / 2.0f), m_Trans.y + (m_size.y / 2.0f), m_Trans.z + (m_size.z / 2.0f));
	Pos6 = Vector3(m_Trans.x - (m_size.x / 2.0f), m_Trans.y - (m_size.y / 2.0f), m_Trans.z + (m_size.z / 2.0f));
	Pos7 = Vector3(m_Trans.x + (m_size.x / 2.0f), m_Trans.y - (m_size.y / 2.0f), m_Trans.z + (m_size.z / 2.0f));
}
