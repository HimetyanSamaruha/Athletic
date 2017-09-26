#pragma once
#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>

#include "Camera.h"
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include <Keyboard.h>
#include <vector>
#include "Obj3d.h"




class Player 
{
public:
	// 1�t���[���ł̊p�x�ω�����<�x�����W�A��>
	static const float ANGLE_DELTA_MAX;

	//���@�p�[�c
	enum PLAYER_PARTS
	{
		PLAYER_PARTS_BODY,

		PLAYER_PARTS_NUM,
	};

	Player(DirectX::Keyboard* keyboard);
	~Player();

	void Init();
	void Update();
	void Render();

	void Advance();
	void Back();
	void LeftRotation();
	void RightRotation();
	void UpRotation();
	void DownRotation();

	//�X�P�[�����O�iXYZ)
	DirectX::SimpleMath::Vector3 Get_scale();
	//��]�p(XYZ�j
	float Get_rotate();
	//���s�ړ��iXYZ�j
	DirectX::SimpleMath::Vector3 Get_transmat();

	DirectX::SimpleMath::Matrix Get_world();



private:
	bool m_attack;			//�U���t���O
	int m_attack_cnt;		//�U���J�E���g

	bool m_defense;			//�h��t���O
	int m_defense_cnt;		//�h��J�E���g

	bool m_jump;			//�W�����v�t���O
	int m_jump_cnt;			//�W�����v�J�E���g
	float jumping;			//�����x�N�g���̏�����
	DirectX::SimpleMath::Vector3 vec;

	bool collision;			//�����蔻��̕\��

	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;

	std::vector<Obj3d> m_ObjPlayer;

	//�L�[�{�[�h
	DirectX::Keyboard* keyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> keyTracker;


	//�e�ۂ̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 m_BulletVel;


};

