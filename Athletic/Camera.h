#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>

class Camera
{
public:
	// �����o�֐�
	// �R���X�g���N�^
	Camera(int width,int height);

	// �f�X�g���N�^
	virtual ~Camera();

	// �X�V
	virtual void Update();

	// �r���[�s����擾
	DirectX::SimpleMath::Matrix GetViewMatrix();

	// �ˉe�s����擾
	DirectX::SimpleMath::Matrix GetProjectionMatrix();

	void Seteyepos(DirectX::SimpleMath::Vector3 eyepos);
	void Setrefpos(DirectX::SimpleMath::Vector3 refpos);
	void Setupvec(DirectX::SimpleMath::Vector3 upvec);
	void SetfovY(float fovY);
	void Setaspect(float aspect);
	void SetnearClip(float nearClip);
	void SetfarClip(float farClip);

protected:
	// �����o�ϐ�

	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// ���_
	DirectX::SimpleMath::Vector3 m_eyepos;
	// �Q�Ɠ_/�����_ 
	DirectX::SimpleMath::Vector3 m_refpos;
	// ������x�N�g��
	DirectX::SimpleMath::Vector3 m_upvec;
	// ������������p
	float m_fovY;
	// �A�X�y�N�g��i���E�c�̔䗦�j
	float m_aspect;
	// �j�A�N���b�v�i��O�̕\�����E�����j
	float m_nearClip;
	// �t�@�[�N���b�v�i���̕\�����E�����j
	float m_farClip;
};