#include "pch.h"

#include "Floor2.h"
#include "Manager.h"
#include <d3d11.h>
#include "SimpleMath.h"

#include "Camera.h"
#include "FollowCamera.h"
#include "Obj3d.h"

#include "Key.h"




using namespace DirectX;
using namespace DirectX::SimpleMath;


SceneBase* Floor2::m_base = NULL;

SceneBase * Floor2::GetInstance()
{
	m_base = new Floor2();

	return m_base;
}

Floor2::Floor2()
{
	this->Initialize();
}


Floor2::~Floor2()
{


}

void Floor2::Initialize()
{

	wall = 46;
	kaidan = 12;
	Draw& draw = Draw::GetInstance();
	Key& key = Key::GetInstance();

	//�J�����̐���
	m_Camera = std::make_unique<FollowCamera>(800, 600);
	
	//�Rd�I�u�W�F�N�g�̐ÓI�����o��������
	Obj3d::InitializeStatic(draw.m_d3dDevice, draw.m_d3dContext, m_Camera.get());
	
	//PrimitiveBatch�̏�����
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(draw.m_d3dContext);

	m_effect = std::make_unique<BasicEffect>(draw.m_d3dDevice);

	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		800, 600, 0, 0, 1));
	m_view = Matrix::CreateLookAt(Vector3(0.f, 2.f, 2.f),
		Vector3(1, 0, 0), Vector3::UnitY);
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(800) / float(600), 0.1f, 500.f);

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	draw.m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	//�G�t�F�N�g�t�@�N�g���[����
	m_factory = std::make_unique<EffectFactory>(draw.m_d3dDevice);
	//�e�N�X�`���̓ǂݍ��݃p�X�w��
	m_factory->SetDirectory(L"Resource");
	//���f���̓ǂݍ���
	m_obj_ground.LoadModel(L"Resource/graund200m.cmo");
	//�V�����f���̓ǂݍ���
	m_obj_skydome.LoadModel(L"Resource/skydome.cmo");


	//�n�`���f���̓ǂݍ���
	for (int i = 0; i < wall; i++)
	{
		m_obj_box[i].LoadModel(L"Resource/box.cmo");
		m_obj_box[i].Set_scale(Vector3(1, 12, 1));
	}

	for (int i = 0; i < kaidan; i++)
	{
		m_kaidan[i].LoadModel(L"Resource/box.cmo");
		m_kaidan[i].Set_scale(Vector3(1.5, 0.3, 1.5));
	}
	//�v���C���[�̐���
	m_player = std::make_unique<Player>(key.m_keyboard.get());

	//�v���C���[���J�����ɃZ�b�g����
	m_Camera->SetPlayer(m_player.get());

	Map();
	Kaidan();
}

void Floor2::Update(Manager * main)
{
	//Key& key = Key::GetInstance();
	//auto kb = key.m_keyboard->GetState();

	//���@�ɃJ�������_�����Ă���
	{
		m_Camera->Update();
		m_view = m_Camera->GetViewMatrix();
		m_proj = m_Camera->GetProjectionMatrix();
	}

	//for (std::vector<std::unique_ptr<ENEMY>>::iterator it = m_enemy.begin(); it != m_enemy.end(); it++)
	//{
	//	(*it)->Update(m_player.get());
	//}


	m_obj_skydome.Update();
	m_obj_ground.Update();


	//�n�`���f���̓ǂݍ���
	for (int i = 0; i < wall; i++)
	{
		m_obj_box[i].Update();
	}

	for (int i = 0; i < kaidan; i++)
	{
		m_kaidan[i].Update();
	}


	m_player->Update();

}


void Floor2::Render()
{
	Draw& draw = Draw::GetInstance();

	//draw.RenderFont(L"Play");

	//���t���[���`�揈��
	DirectX::CommonStates m_states(draw.m_d3dDevice);
	draw.m_d3dContext->OMSetBlendState(m_states.Opaque(), nullptr, 0xFFFFFFFF);
	draw.m_d3dContext->OMSetDepthStencilState(m_states.DepthNone(), 0);
	draw.m_d3dContext->RSSetState(m_states.CullNone());

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);
	m_effect->SetWorld(m_world);
	m_effect->Apply(draw.m_d3dContext);
	draw.m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	////�V�����f���̕`��
	m_obj_skydome.Draw();

	////�n�ʃ��f���̕`��
	m_obj_ground.Draw();



	//�n�`���f���̓ǂݍ���
	for (int i = 0; i < wall; i++)
	{
		m_obj_box[i].Draw();
	}

	for (int i = 0; i < kaidan; i++)
	{
		m_kaidan[i].Draw();
	}

	m_player->Render();


}

void Floor2::Dispose()
{
	if (m_base != NULL)
	{
		delete m_base;
	}
}

void Floor2::Map()
{

	m_obj_box[0].Set_trans(Vector3(3, 0, 0));
	m_obj_box[1].Set_trans(Vector3(3, 0, -1));
	m_obj_box[2].Set_trans(Vector3(3, 0, -2));
	m_obj_box[3].Set_trans(Vector3(3, 0, -3));
	m_obj_box[4].Set_trans(Vector3(3, 0, -4));
	m_obj_box[5].Set_trans(Vector3(3, 0, -5));
	m_obj_box[6].Set_trans(Vector3(3, 0, -6));
	m_obj_box[7].Set_trans(Vector3(3, 0, -7));
	m_obj_box[8].Set_trans(Vector3(3, 0, -8));
	m_obj_box[9].Set_trans(Vector3(3, 0, -9));
	m_obj_box[10].Set_trans(Vector3(3, 0, -10));

	m_obj_box[11].Set_trans(Vector3(-1, 0, 0));
	m_obj_box[12].Set_trans(Vector3(-1, 0, -1));
	m_obj_box[13].Set_trans(Vector3(-1, 0, -2));
	m_obj_box[14].Set_trans(Vector3(-1, 0, -3));
	m_obj_box[15].Set_trans(Vector3(-1, 0, -4));
	m_obj_box[16].Set_trans(Vector3(-1, 0, -5));
	m_obj_box[17].Set_trans(Vector3(-1, 0, -6));
	m_obj_box[18].Set_trans(Vector3(-1, 0, -7));
	m_obj_box[19].Set_trans(Vector3(-1, 0, -8));
	m_obj_box[20].Set_trans(Vector3(-1, 0, -9));
	m_obj_box[21].Set_trans(Vector3(-1, 0, -10));

	m_obj_box[22].Set_trans(Vector3(3, 0, -11));
	m_obj_box[24].Set_trans(Vector3(3, 0, -12));
	m_obj_box[25].Set_trans(Vector3(3, 0, -13));
	m_obj_box[26].Set_trans(Vector3(3, 0, -14));
	m_obj_box[27].Set_trans(Vector3(3, 0, -15));
	m_obj_box[28].Set_trans(Vector3(3, 0, -16));
	m_obj_box[29].Set_trans(Vector3(3, 0, -17));
	m_obj_box[30].Set_trans(Vector3(3, 0, -18));
	m_obj_box[31].Set_trans(Vector3(3, 0, -19));

	m_obj_box[32].Set_trans(Vector3(-1, 0, -11));
	m_obj_box[33].Set_trans(Vector3(-1, 0, -12));
	m_obj_box[23].Set_trans(Vector3(-1, 0, -13));
	m_obj_box[34].Set_trans(Vector3(-1, 0, -14));
	m_obj_box[35].Set_trans(Vector3(-1, 0, -15));
	m_obj_box[36].Set_trans(Vector3(-1, 0, -16));
	m_obj_box[37].Set_trans(Vector3(-1, 0, -17));
	m_obj_box[38].Set_trans(Vector3(-1, 0, -18));
	m_obj_box[39].Set_trans(Vector3(-1, 0, -19));

	m_obj_box[40].Set_trans(Vector3(-1, 0, -20));
	m_obj_box[41].Set_trans(Vector3(-1, 0, -21));
	m_obj_box[42].Set_trans(Vector3(3, 0, -20));
	m_obj_box[43].Set_trans(Vector3(3, 0, -21));
	m_obj_box[44].Set_trans(Vector3(3, 0, -22));
	m_obj_box[45].Set_trans(Vector3(-1, 0, -22));
	//m_obj_box[46].Set_trans(Vector3(3, 0, 0));
	//m_obj_box[47].Set_trans(Vector3(3, 0, 0));
	//m_obj_box[48].Set_trans(Vector3(3, 0, 0));
	//m_obj_box[49].Set_trans(Vector3(3, 0, 0));
	//m_obj_box[50].Set_trans(Vector3(3, 0, 0));
	//m_obj_box[51].Set_trans(Vector3(3, 0, 0));
	//m_obj_box[52].Set_trans(Vector3(3, 0, 0));
	//m_obj_box[53].Set_trans(Vector3(3, 0, 0));
	//m_obj_box[54].Set_trans(Vector3(3, 0, 0));
}

void Floor2::Kaidan()
{
	m_kaidan[0].Set_trans(Vector3(1, 1.5, -1));
	m_kaidan[1].Set_trans(Vector3(1, 2, -3));
	m_kaidan[2].Set_trans(Vector3(1, 3, -5));
	m_kaidan[3].Set_trans(Vector3(1, 4, -7));
	m_kaidan[4].Set_trans(Vector3(1, 5, -9));
	m_kaidan[5].Set_trans(Vector3(1, 6, -11));
	m_kaidan[6].Set_trans(Vector3(1, 7, -13));
	m_kaidan[7].Set_trans(Vector3(1, 8, -15));
	m_kaidan[8].Set_trans(Vector3(1, 9, -17));
	m_kaidan[9].Set_trans(Vector3(1, 10, -19));
	m_kaidan[10].Set_trans(Vector3(1, 10, -20));
	m_kaidan[11].Set_trans(Vector3(1, 10, -21));
}