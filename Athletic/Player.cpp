#include "pch.h"
#include "Player.h"
#include <stdlib.h>


using namespace DirectX;
using namespace DirectX::SimpleMath;

// 1�t���[���ł̊p�x�ω�����<�x�����W�A��>
const float Player::ANGLE_DELTA_MAX = DirectX::XMConvertToRadians(30.0f);
float Player::GRAVITY = 0.03f;

//��----------------------------------------------------��
//��*func�F�R���X�g���N�^
//��*arg�F�Ȃ�
//��----------------------------------------------------��
Player::Player(DirectX::Keyboard* keyboard, int id)
{
	m_ObjPlayer.resize(PLAYER_PARTS_NUM);

	//���@�p�[�c�̓ǂݍ���
	switch (id)
	{
	case CUPSULE:
		m_ObjPlayer[PLAYER_PARTS_BODY].LoadModel(L"Resource/player.cmo");

		break;
	case SPHERE:
		m_ObjPlayer[PLAYER_PARTS_BODY].LoadModel(L"Resource/sphere.cmo");
		m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(Vector3(0, 0.5, -10));
		break;
	case CUBE:
		m_ObjPlayer[PLAYER_PARTS_BODY].LoadModel(L"Resource/boxNode.cmo");
		this->SetTrans(Vector3(0, 1, -10));
		break;
	default:
		m_ObjPlayer[PLAYER_PARTS_BODY].LoadModel(L"Resource/player.cmo");
		m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(Vector3(0, 0, 0));
		break;
	}
	//m_ObjPlayer[PLAYER_PARTS_BODY].LoadModel(L"Resource/player.cmo");
	//m_ObjPlayer[PLAYER_PARTS_HEAD].LoadModel(L"Resources/head.cmo");
	//m_ObjPlayer[PLAYER_PARTS_DOWNBODY].LoadModel(L"Resources/down_body.cmo");



	////�e����̃I�t�Z�b�g
	//m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(Vector3(1, 0, 0));

	m_ObjPlayer[PLAYER_PARTS_BODY].Set_scale(Vector3(1, 1, 1));
	//m_ObjPlayer[PLAYER_PARTS_BODY].Set_rotate(Vector3(0, 10, 0));

	//�����蔻��͕`�悳��Ȃ�
	isCollision = true;

	//���[�ځ[�ǂ̏�����
	this->keyboard = keyboard;
	keyTracker = std::make_unique<Keyboard::KeyboardStateTracker>();
	std::unique_ptr<Keyboard::KeyboardStateTracker> keyTracker(new Keyboard::KeyboardStateTracker);

	m_jump = true;
	jumping = 0;

	m_BoxN.Initialize();

	segment.Start = Vector3(0, 0.5f, 0);
	segment.End = Vector3(0, 2.0f, 0);

	m_playerCapsule.Radius = 0.5f;
	m_playerCapsule.Segment = segment;

	m_sphereN.Initialize();
	m_sphereN.SetLocalRadius(0.5f);

	rollCnt = 0;
	WalkCase = 0;
}

//��----------------------------------------------------��
//��*func�F�f�X�g���N�^
//��*arg�F�Ȃ�
//��*return�F�Ȃ�
//��----------------------------------------------------��
Player::~Player()
{
}


void Player::Init()
{
}

//��----------------------------------------------------��
//��*func�F��i
//��*arg�F����I�u�W�F�N�g,�e�I�u�W�F�N�g
//��*return�F�Ȃ�
//��----------------------------------------------------��
void Player::Update()
{
	//�L�[�{�[�h�̏��擾
	Keyboard::State key = keyboard->GetState();
	keyTracker->Update(key);

	//A�L�[�������ꂽ��
	if (key.Left && rollCnt == 0)
	{
		WalkCase = 0;
		rollCnt = 20;
	}
	//D�L�[�������ꂽ��
	if (key.Right && rollCnt == 0)
	{
		WalkCase = 1;
		rollCnt = 20;
	}

	//��L�[
	if (key.Up && rollCnt == 0)
	{
		WalkCase = 2;
		rollCnt = 20;
	}

	//���L�[
	if (key.Down && rollCnt == 0)
	{
		WalkCase = 3;
		rollCnt = 20;

	}
	//W�L�[�������ꂽ��
	if (key.A && rollCnt == 0)
	{
		WalkCase = 5;
		rollCnt = 20;
	}
	//S�L�[�������ꂽ��
	if (key.D && rollCnt == 0)
	{
		WalkCase = 5;
		rollCnt = -20;
	}

	//�X�y�[�X�L�[�������ꂽ��ijump�j
	if (keyTracker->pressed.Space && m_jump)
	{
		Jumping();
	}

	Jump();

	if (m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat().y <= 0.5f)
	{
		m_jump = true;
		Vector3 vec = m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat();
		vec.y = 0.5f;
		this->SetTrans(vec);
	}



	if (rollCnt != 0)
	{
		switch (WalkCase)
		{
		case 0:
			rollCnt--;
			Left();
			break;
		case 1:
			rollCnt--;
			Right();
			break;
		case 2:
			rollCnt--;
			Advance();
			break;
		case 3:
			rollCnt--;
			Back();
			break;
		default:
			if (rollCnt < 0) { RightRotation(); rollCnt++; }
			else { LeftRotation(); rollCnt--; }
			break;
		}
	}
	else {
		WalkCase = 99;
	}
	m_sphereN.SetTrans(this->Get_transmat());
	m_BoxN.SetTrans(this->Get_transmat());


	Colc();
}

void Player::Render()
{
	for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Draw();
	}

	m_BoxN.Render();
	m_sphereN.Render();
}

void Player::Colc()
{
	for (std::vector<Obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Update();
	}

	m_BoxN.Update();
	m_sphereN.Update();
}

//��----------------------------------------------------��
//��*func�F�O�i
//��*arg�F�e�I�u�W�F�N�g
//��*return�F�Ȃ�
//��----------------------------------------------------��
void Player::Advance()
{
	Vector3 moveV(0, 0, -(1 / 20.0f));
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;

	Matrix rotmat = Matrix::CreateRotationY(angle);
	moveV = Vector3::TransformNormal(moveV, rotmat);

	Vector3 pos = m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat();
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(pos + moveV);
}

//��----------------------------------------------------��
//��*func�F��i
//��*arg�F�e�I�u�W�F�N�g
//��*return�F�Ȃ�
//��----------------------------------------------------��
void Player::Back()
{
	Vector3 moveV(0, 0, 1 / 20.0f);
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;

	Matrix rotmat = Matrix::CreateRotationY(angle);
	moveV = Vector3::TransformNormal(moveV, rotmat);

	Vector3 pos = m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat();
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(pos + moveV);

}

//��----------------------------------------------------��
//��*func�F��
//��*arg�F�e�I�u�W�F�N�g
//��*return�F�Ȃ�
//��----------------------------------------------------��
void Player::Left()
{
	Vector3 moveV(-(1 / 20.0f), 0, 0);
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;

	Matrix rotmat = Matrix::CreateRotationY(angle);
	moveV = Vector3::TransformNormal(moveV, rotmat);

	Vector3 pos = m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat();
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(pos + moveV);
}

//��----------------------------------------------------��
//��*func�F�E
//��*arg�F�e�I�u�W�F�N�g
//��*return�F�Ȃ�
//��----------------------------------------------------��
void Player::Right()
{
	Vector3 moveV(1 / 20.0f, 0, 0);
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;

	Matrix rotmat = Matrix::CreateRotationY(angle);
	moveV = Vector3::TransformNormal(moveV, rotmat);

	Vector3 pos = m_ObjPlayer[PLAYER_PARTS_BODY].Get_transmat();
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_trans(pos + moveV);

}


//��----------------------------------------------------��
//��*func�F����]
//��*arg�F�e�I�u�W�F�N�g
//��*return�F�Ȃ�
//��----------------------------------------------------��
void Player::LeftRotation()
{
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;
	float angle_x = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().x;
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_rotate(Vector3(angle_x, angle + XMConvertToRadians(4.5f), 0));
}

//��----------------------------------------------------��
//��*func�F�E��]
//��*arg�F�e�I�u�W�F�N�g
//��*return�F�Ȃ�
//��----------------------------------------------------��
void Player::RightRotation()
{
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;
	float angle_x = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().x;
	m_ObjPlayer[PLAYER_PARTS_BODY].Set_rotate(Vector3(angle_x, angle - XMConvertToRadians(4.5f), 0));
}

//��----------------------------------------------------��
//��*func�F���]
//��*arg�F�e�I�u�W�F�N�g
//��*return�F�Ȃ�
//��----------------------------------------------------��
void Player::UpRotation()
{
	float angle = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().x;
	float angle_y = m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().y;
	if (m_ObjPlayer[PLAYER_PARTS_BODY].Get_rotate().x < 0.15)
	{
		m_ObjPlayer[PLAYER_PARTS_BODY].Set_rotate(Vector3(angle + 0.005f, angle_y, 0));
	}
}
//��----------------------------------------------------��
//��*func�F����]
//��*arg�F�e�I�u�W�F�N�g
//��*return�F�Ȃ�
//��----------------------------------------------------��
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
/// �W�����v����
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

BoxNode& Player::GetBoxNode()
{
	return m_BoxN;
}

Capsule Player::GetCapsule()
{
	return m_playerCapsule;
}

SphereNode& Player::GetSphere()
{
	return m_sphereN;
}

void Player::StopMove()
{
	if (m_jump) { m_jump = !m_jump; }
	switch (WalkCase)
	{
	case 0:
		Right();
		break;
	case 1:
		Left();
		break;
	case 2:
		Back();
		break;
	case 3:
		Advance();
		break;
	}
}