//��----------------------------------------------------��
//��*�t�@�C�����FObj3d.cpp								��
//��*���e�@�@�@�F3D�I�u�W�F�N�g�̃N���X					��
//��*����ҁ@�@�FAyaka Yamanaka							��
//��*��������@�F2017.05.22								��
//��----------------------------------------------------��
#include "pch.h"

#include "Obj3d.h"
//#include <d3d11_1.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>           Obj3d::m_d3dDevice;
//�f�o�C�X�R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;
//�J����
Camera* Obj3d::m_Camera;
//�ėp�`��X�e�[�g
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;
//�G�t�F�N�g�t�@�N�g���[
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_Camera = camera;

	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	//�G�t�F�N�g�t�@�N�g���[����
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//�e�N�X�`���̓ǂݍ��݃p�X�w��
	m_factory->SetDirectory(L"Resources");

}

Obj3d::Obj3d()
{
	//�X�P�[���͂P�{���f�t�H���g
	m_scalemat = SimpleMath::Vector3(1, 1, 1);

	m_perent = nullptr;

	//�f�t�H���g�ł̓I�C���[�p�Ōv�Z
	m_useQuaternion = false;
}

void Obj3d::LoadModel(const wchar_t * filename)
{
	//���f���̓ǂݍ���
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), filename, *m_factory);

}

void Obj3d::Update()
{
	//��ɍs��̌v�Z
	SimpleMath::Matrix scalemat = SimpleMath::Matrix::CreateScale(m_scalemat);

	SimpleMath::Matrix rotmat;
	//��]�s��
	if (m_useQuaternion)
	{//�N�H�[�^�j�I���ŉ�]���v�Z
		rotmat = Matrix::CreateFromQuaternion(m_rotmatQ);
	}
	else
	{//�I�C���[�p�ŉ�]���v�Z�iZ��X��Y�j
		SimpleMath::Matrix rotmatZ = SimpleMath::Matrix::CreateRotationZ(m_rotmat.z);
		SimpleMath::Matrix rotmatX = SimpleMath::Matrix::CreateRotationX(m_rotmat.x);
		SimpleMath::Matrix rotmatY = SimpleMath::Matrix::CreateRotationY(m_rotmat.y);

		rotmat = rotmatZ * rotmatX * rotmatY;
	}

	SimpleMath::Matrix transmat = SimpleMath::Matrix::CreateTranslation(m_transmat);

	//���[���h�s�������
	m_world = scalemat*rotmat*transmat;

	//�e�̍s���������
	if (m_perent)
	{
		m_world *= m_perent->m_world;
	}

}

void Obj3d::Draw()
{
	assert(m_Camera);
	//���f���̕`��
	if (m_model)
	{
		m_model->Draw(m_d3dContext.Get(),
			*m_states,
			m_world,
			m_Camera->GetViewMatrix(),
			m_Camera->GetProjectionMatrix());
	}
}

void Obj3d::Set_scale(DirectX::SimpleMath::Vector3 scalemat)
{
	m_scalemat = scalemat;
}

void Obj3d::Set_rotate(DirectX::SimpleMath::Vector3 rotmat)
{
	m_rotmat = rotmat;
	m_useQuaternion = false;
}

void Obj3d::Set_rotateQ(DirectX::SimpleMath::Quaternion rotmat)
{
	m_rotmatQ = rotmat;
	m_useQuaternion = true;
}

void Obj3d::Set_world(DirectX::SimpleMath::Matrix world)
{
	m_world = world;
}


void Obj3d::Set_trans(DirectX::SimpleMath::Vector3 transmat)
{
	m_transmat = transmat;
}

void Obj3d::Set_perant(Obj3d * perant)
{
	m_perent = perant;
}

DirectX::SimpleMath::Vector3 Obj3d::Get_scale()
{
	return m_scalemat;
}

DirectX::SimpleMath::Vector3 Obj3d::Get_rotate()
{
	return m_rotmat;
}

DirectX::SimpleMath::Vector3 Obj3d::Get_transmat()
{
	return m_transmat;
}

DirectX::SimpleMath::Quaternion Obj3d::Get_Qua()
{
	return m_rotmatQ;
}

Obj3d * Obj3d::Get_perant()
{
	return m_perent;
}

DirectX::SimpleMath::Matrix Obj3d::Get_world()
{
	return m_world;
}

void Obj3d::DisableLighting()

{

	if (m_model)

	{

		// ���f�����̑S���b�V������

		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();

		for (; it_mesh != m_model->meshes.end(); it_mesh++)

		{

			ModelMesh* modelmesh = it_mesh->get();

			assert(modelmesh);



			// ���b�V�����̑S���b�V���p�[�c����

			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();

			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)

			{

				ModelMeshPart* meshpart = it_meshpart->get();

				assert(meshpart);



				// ���b�V���p�[�c�ɃZ�b�g���ꂽ�G�t�F�N�g��BasicEffect�Ƃ��Ď擾

				std::shared_ptr<IEffect> ieff = meshpart->effect;

				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());

				if (eff != nullptr)

				{

					// ���Ȕ������ő�l��

					eff->SetEmissiveColor(Vector3(1, 1, 1));



					// �G�t�F�N�g�Ɋ܂ޑS�Ă̕��s�������ɂ��ď�������

					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)

					{

						// ���C�g�𖳌��ɂ���

						eff->SetLightEnabled(i, false);

					}

				}

			}

		}

	}

}

