#include "pch.h"
#include "Player.h"
#include <stdlib.h>


using namespace DirectX;
using namespace DirectX::SimpleMath;

// 1フレームでの角度変化制限<度→ラジアン>
const float Player::ANGLE_DELTA_MAX = DirectX::XMConvertToRadians(30.0f);
float Player::GRAVITY = 0.03f;

//∞----------------------------------------------------∞
//∞*func：コンストラクタ
//∞*arg：なし
//∞----------------------------------------------------∞
Player::Player(DirectX::Keyboard* keyboard, int id)
{
	m_ObjPlayer.resize(PLAYER_PARTS_NUM);

	//自機パーツの読み込み
	switch (id)
	{
	case CUPSULE:
		m_ObjPlayer[PLAYER_PARTS_BODY].LoadModel(L"Resource/player.cmo");
		m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(Vector3(1, 0, 0));
		break;
	case SPHERE:
		m_ObjPlayer[PLAYER_PARTS_BODY].LoadModel(L"Resource/sphere.cmo");
		m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(Vector3(1, 0.5, 0));
		break;
	case CUBE:
		m_ObjPlayer[PLAYER_PARTS_BODY].LoadModel(L"Resource/box.cmo");
		m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(Vector3(1, 0, 0));
		break;
	default:
		m_ObjPlayer[PLAYER_PARTS_BODY].LoadModel(L"Resource/player.cmo");
		m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(Vector3(1, 0, 0));
		break;
	}
	//m_ObjPlayer[PLAYER_PARTS_BODY].LoadModel(L"Resource/player.cmo");
	//m_ObjPlayer[PLAYER_PARTS_HEAD].LoadModel(L"Resources/head.cmo");
	//m_ObjPlayer[PLAYER_PARTS_DOWNBODY].LoadModel(L"Resources/down_body.cmo");



	////親からのオフセット
	//m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(Vector3(1, 0, 0));

	m_ObjPlayer[PLAYER_PARTS_BODY].Set_scale(Vector3(1, 1, 1));
	//m_ObjPlayer[PLAYER_PARTS_BODY].Set_rotate(Vector3(0, 10, 0));

	//当たり判定は描画されない
	isCollision = true;

	//きーぼーどの初期化
	this->keyboard = keyboard;
	keyTracker = std::make_unique<Keyboard::KeyboardStateTracker>();
	std::unique_ptr<Keyboard::KeyboardStateTracker> keyTracker(new Keyboard::KeyboardStateTracker);

	m_jump = true;
	jumping = 0;

<<<<<<< HEAD
	m_BoxN.Initialize();
=======
	segment.Start = Vector3(0, 0.5f, 0);
	segment.End = Vector3(0, 2.0f, 0);

	m_playerCapsule.Radius = 0.5f;
	m_playerCapsule.Segment = segment;

	//m_sphereN.Initialize();
	//m_sphereN.SetLocalRadius(0.5f);
>>>>>>> map
}

//∞----------------------------------------------------∞
//∞*func：デストラクタ
//∞*arg：なし
//∞*return：なし
//∞----------------------------------------------------∞
Player::~Player()
{
}


void Player::Init()
{
}

//∞----------------------------------------------------∞
//∞*func：後進
//∞*arg：武器オブジェクト,親オブジェクト
//∞*return：なし
//∞----------------------------------------------------∞
void Player::Update()
{
	//キーボードの情報取得
	Keyboard::State key = keyboard->GetState();
	keyTracker->Update(key);

	//Aキーが押されたら
	if (key.Left)
	{
		Left();
	}
	//Dキーが押されたら
	if (key.Right)
	{
		Right();
	}

	//上キー
	if (key.Up)
	{
		Advance();
	}

	//下キー
	if (key.Down)
	{
		Back();
	}
	//Wキーが押されたら
	if (key.A)
	{
		LeftRotation();
	}
	//Sキーが押されたら
	if (key.D)
	{
		RightRotation();
	}

	//スペースキーが押されたら（jump）
	if (keyTracker->pressed.Space && m_jump)
	{
		Jumping();
	}

	if (!m_jump)
	{
		Jump();
	}

	if (m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat().y <= 0) 
	{
		m_jump = true;
		Vector3 vec = m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat();
		vec.y = 0;
		m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(vec);
	}

	for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Update();
	}

<<<<<<< HEAD
	m_BoxN.SetTrans(this->Get_transmat());
	m_BoxN.Update();
=======
	//m_sphereN.SetTrans(this->Get_transmat());
	//m_sphereN.Update();
>>>>>>> map
}

void Player::Render()
{
	for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Draw();
	}

<<<<<<< HEAD
	m_BoxN.Render();
=======
	//m_sphereN.Render();
>>>>>>> map

}

//∞----------------------------------------------------∞
//∞*func：前進
//∞*arg：親オブジェクト
//∞*return：なし
//∞----------------------------------------------------∞
void Player::Advance()
{
	Vector3 moveV(0, 0, -0.1f);
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;

	Matrix rotmat = Matrix::CreateRotationY(angle);
	moveV = Vector3::TransformNormal(moveV, rotmat);

	Vector3 pos = m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat();
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(pos + moveV);
}

//∞----------------------------------------------------∞
//∞*func：後進
//∞*arg：親オブジェクト
//∞*return：なし
//∞----------------------------------------------------∞
void Player::Back()
{
	Vector3 moveV(0, 0, 0.1f);
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;

	Matrix rotmat = Matrix::CreateRotationY(angle);
	moveV = Vector3::TransformNormal(moveV, rotmat);

	Vector3 pos = m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat();
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(pos + moveV);

}

//∞----------------------------------------------------∞
//∞*func：左
//∞*arg：親オブジェクト
//∞*return：なし
//∞----------------------------------------------------∞
void Player::Left()
{
	Vector3 moveV(-0.1f, 0, 0);
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;

	Matrix rotmat = Matrix::CreateRotationY(angle);
	moveV = Vector3::TransformNormal(moveV, rotmat);

	Vector3 pos = m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat();
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(pos + moveV);
}

//∞----------------------------------------------------∞
//∞*func：右
//∞*arg：親オブジェクト
//∞*return：なし
//∞----------------------------------------------------∞
void Player::Right()
{
	Vector3 moveV(0.1f, 0, 0);
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;

	Matrix rotmat = Matrix::CreateRotationY(angle);
	moveV = Vector3::TransformNormal(moveV, rotmat);

	Vector3 pos = m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat();
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(pos + moveV);

}


//∞----------------------------------------------------∞
//∞*func：左回転
//∞*arg：親オブジェクト
//∞*return：なし
//∞----------------------------------------------------∞
void Player::LeftRotation()
{
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;
	float angle_x = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().x;
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_rotate(Vector3(angle_x, angle + 0.01f, 0));
}

//∞----------------------------------------------------∞
//∞*func：右回転
//∞*arg：親オブジェクト
//∞*return：なし
//∞----------------------------------------------------∞
void Player::RightRotation()
{
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;
	float angle_x = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().x;
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_rotate(Vector3(angle_x, angle - 0.01f, 0));
}

//∞----------------------------------------------------∞
//∞*func：上回転
//∞*arg：親オブジェクト
//∞*return：なし
//∞----------------------------------------------------∞
void Player::UpRotation()
{
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().x;
	float angle_y = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;
	if (m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().x < 0.15)
	{
		m_ObjPlayer[PLAYER_PARTS_BODY].Set_rotate(Vector3(angle + 0.005f, angle_y, 0));
	}
}
//∞----------------------------------------------------∞
//∞*func：下回転
//∞*arg：親オブジェクト
//∞*return：なし
//∞----------------------------------------------------∞
void Player::DownRotation()
{
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().x;
	float angle_y = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;

	if (m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().x > -0.09)
	{
		m_ObjPlayer[PLAYER_PARTS_BODY].Set_rotate(Vector3(angle - 0.005f, angle_y, 0));
	}

}

/// <summary>
/// 
/// ジャンプする
/// 
/// </summary>
void Player::Jumping()
{
	if (!m_jump) 
	{
		return;
	}

	m_jump = !m_jump;
	jumping = 0.5f;
}

void Player::Jump()
{
	jumping -= GRAVITY;

	Vector3 moveV(0, jumping, 0);
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;

	Matrix rotmat = Matrix::CreateRotationY(angle);
	moveV = Vector3::TransformNormal(moveV, rotmat);

	Vector3 pos = m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat();
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(pos + moveV);

}


DirectX::SimpleMath::Vector3 Player::Get_scale()
{
	return m_ObjPlayer[PLAYER_PARTS_BODY].Get_scale();
}

float Player::Get_rotate()
{
	return m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;
}

DirectX::SimpleMath::Vector3 Player::Get_transmat()
{
	return m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat();
}
DirectX::SimpleMath::Matrix Player::Get_world()
{
	return m_ObjPlayer[PLAYER_PARTS_BODY].Get_world();
}

const BoxNode& Player::GetBoxNode()
{
	return m_BoxN;
}

Capsule Player::GetCapsule()
{
	return m_playerCapsule;
}


