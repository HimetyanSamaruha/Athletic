#pragma once
#include "Manager.h"
#include "Draw.h"
#include "Camera.h"
#include "FollowCamera.h"
#include "Player.h"


#include "Collision.h"
#include "CollisionNode.h"

#include <Effects.h>
#include <d3d11.h>
#include "SimpleMath.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"


class Floor1 :
	public SceneBase
{
public:
	static SceneBase* GetInstance();
	Floor1();

	~Floor1();
	void Initialize();
	void Update(Manager* main);	//	データの更新
	void Render();	//	描画する
	static void Dispose();
	void SceneChange(Manager * main);

	void Map();

	static float GRAVITY;

private:
	static SceneBase* m_base;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;

	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	//地面モデル
	Obj3d m_obj_ground;
	//天球モデル
	Obj3d m_obj_skydome;

	//動かすモデル
	Obj3d m_obj_move;
	SphereNode MoveObjectNode;

	//地形モデル
	Obj3d m_obj_box[69];
	BoxNode m_groundBox[69];

	//判定場所
	Obj3d m_obj_judge;

	Obj3d m_Box;
	BoxNode m_BNode;

	float BoundBallSpd;

	//カメラ
	std::unique_ptr<FollowCamera> m_Camera;

	//エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_factory;

	std::unique_ptr<Player> m_player;

public:
	static int wall;

};

