//��----------------------------------------------------��
//��*�t�@�C�����FObj3d.h								��
//��*���e�@�@�@�F3D�I�u�W�F�N�g�̃N���X					��
//��*����ҁ@�@�FAyaka Yamanaka							��
//��*��������@�F2017.05.22								��
//��----------------------------------------------------��
#pragma once

#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>

#include "Camera.h"
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

class Obj3d
{
	//�ÓI�����o
public:
	//�ÓI�����o�ϐ��̏�����
	static void InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera* camera);
protected:
	//�f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	//�f�o�C�X�R���e�L�X�g
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	//�J����
	static Camera* m_Camera;
	//�ėp�`��X�e�[�g
	static std::unique_ptr<DirectX::CommonStates> m_states;
	//�G�t�F�N�g�t�@�N�g���[
	static std::unique_ptr<DirectX::EffectFactory> m_factory;


public:
	//�R���X�g���N�^
	Obj3d();

	//���f�������[�h
	void LoadModel(const wchar_t* filename);

	//�X�V
	void Update();

	//�`��
	void Draw();

	//setter
	//�X�P�[�����O�iXYZ)
	void Set_scale(DirectX::SimpleMath::Vector3 scalemat);
	//��]�p(XYZ�j
	void Set_rotate(DirectX::SimpleMath::Vector3 rotmat);
	//��]�p(�N�H�[�^�j�I���j
	void Set_rotateQ(DirectX::SimpleMath::Quaternion rotmat);
	void Set_world(DirectX::SimpleMath::Matrix world);
	//���s�ړ��iXYZ�j
	void Set_trans(DirectX::SimpleMath::Vector3 transmat);
	//�e��3D�I�u�W�F�N�g�̃|�C���^
	void Set_perant(Obj3d* perant);

	//getter
	//�X�P�[�����O�iXYZ)
	DirectX::SimpleMath::Vector3 Get_scale();
	//��]�p(XYZ�j
	DirectX::SimpleMath::Vector3 Get_rotate();
	//���s�ړ��iXYZ�j
	DirectX::SimpleMath::Vector3 Get_transmat();
	//��]�p
	DirectX::SimpleMath::Quaternion Get_Qua();
	//�e��3D�I�u�W�F�N�g�̃|�C���^
	Obj3d* Get_perant();

	//���[���h�s��
	DirectX::SimpleMath::Matrix Get_world();

	void DisableLighting();

protected:
	//�����o�[�ϐ�
	//3D���f��
	std::unique_ptr<DirectX::Model> m_model;

	//�X�P�[�����O�iXYZ)
	DirectX::SimpleMath::Vector3 m_scalemat;

	//��]�p(XYZ�j
	DirectX::SimpleMath::Vector3 m_rotmat;

	//��]�p(�N�H�[�^�j�I���j
	DirectX::SimpleMath::Quaternion m_rotmatQ;

	//���s�ړ��iXYZ�j
	DirectX::SimpleMath::Vector3 m_transmat;
	
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;

	//�e�̃��[���h�s��̃|�C���^
	Obj3d* m_perent;

	//�N�H�[�^�j�I���g�p�t���O
	bool m_useQuaternion;
	
};

