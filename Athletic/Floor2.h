#pragma once
#include "Manager.h"
#include "Draw.h"
#include "Camera.h"
#include "FollowCamera.h"
#include "Player.h"

#include "CollisionNode.h"


#include <Effects.h>
#include <d3d11.h>
#include "SimpleMath.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"


class Floor2 :
	public SceneBase
{
public:
	static SceneBase* GetInstance();
	Floor2();

	~Floor2();
	void Initialize();
	void Update(Manager* main);	//	�f�[�^�̍X�V
	void Render();	//	�`�悷��
	static void Dispose();

	void Map();
	void Kaidan();

private:
	static SceneBase* m_base;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;

	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	//�n�ʃ��f��
	Obj3d m_obj_ground;
	//�V�����f��
	Obj3d m_obj_skydome;

	//�n�`���f��
	Obj3d m_obj_box[46];
	BoxNode m_WallCollisionNode[46];

	//�K�i
	Obj3d m_kaidan[12];
	BoxNode m_KaidanCollisionNode[12];

	//�J����
	std::unique_ptr<FollowCamera> m_Camera;

	//�G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_factory;

	std::unique_ptr<Player> m_player;

public:
	static int wall;
	static int kaidan;

};

