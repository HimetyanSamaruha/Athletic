#include "pch.h"

#include "Floor1.h"
#include "Floor2.h"
#include "Floor3.h"
#include "Floor4.h"
#include "Floor5.h"
#include "Floor6.h"

#include "Manager.h"
#include <d3d11.h>
#include "SimpleMath.h"

#include "Camera.h"
#include "FollowCamera.h"
#include "Obj3d.h"

#include "Key.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SceneBase* Floor1::m_base = NULL;

int Floor1::wall = 69;

float Floor1::GRAVITY = 0.01f;

SceneBase * Floor1::GetInstance()
{
	m_base = new Floor1();

	return m_base;
}

Floor1::Floor1()
{
	this->Initialize();
}


Floor1::~Floor1()
{


}

void Floor1::Initialize()
{
	Draw& draw = Draw::GetInstance();
	Key& key = Key::GetInstance();

	//カメラの生成
	m_Camera = std::make_unique<FollowCamera>(800, 600);
	
	//３dオブジェクトの静的メンバを初期化
	Obj3d::InitializeStatic(draw.m_d3dDevice, draw.m_d3dContext, m_Camera.get());
	
	//PrimitiveBatchの初期化
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

	//エフェクトファクトリー生成
	m_factory = std::make_unique<EffectFactory>(draw.m_d3dDevice);
	//テクスチャの読み込みパス指定
	m_factory->SetDirectory(L"Resource");
	//モデルの読み込み
	m_obj_ground.LoadModel(L"Resource/graund200m.cmo");
	//天球モデルの読み込み
	m_obj_skydome.LoadModel(L"Resource/skydome.cmo");

	m_obj_move.LoadModel(L"Resource/sphere.cmo");
	m_obj_move.Set_trans(Vector3(1, 0.5f, -4));
	MoveObjectNode.Initialize();
	MoveObjectNode.SetTrans(m_obj_move.Get_transmat());
	MoveObjectNode.SetLocalRadius(0.5f);

	//地形モデルの読み込み
	for (int i = 0; i < wall; i++)
	{
		m_obj_box[i].LoadModel(L"Resource/box.cmo");
		m_obj_box[i].Set_scale(Vector3(1, 6, 1));
		m_groundBox[i].Initialize();
		m_groundBox[i].SetSize(Vector3(1, 6, 1));
	}
	//プレイヤーの生成
	m_player = std::make_unique<Player>(key.m_keyboard.get(),2);

	//プレイヤーをカメラにセットする
	m_Camera->SetPlayer(m_player.get());

	Map();

	// 箱の初期化
	m_Box.LoadModel(L"Resource/boxNode.cmo");
	m_Box.Set_trans(Vector3(0, 2.5f, -10));

	// 箱のあたり判定の初期化
	m_BNode.Initialize();
	m_BNode.SetTrans(m_Box.Get_transmat());

	// 最初のバウンドスピードを0に設定
	BoundBallSpd = 0;
}

void Floor1::Update(Manager * main)
{

	Key& key = Key::GetInstance();
	auto kb = key.m_keyboard->GetState();


	//自機にカメラ視点がついてくる
	{
		m_Camera->Update();
		m_view = m_Camera->GetViewMatrix();
		m_proj = m_Camera->GetProjectionMatrix();
	}
	// 重力を反映
	BoundBallSpd -= GRAVITY;

	// 物体が地面を抜けようとしていたら
	if (MoveObjectNode.GetTrans().y < 0.5f)
	{
		// 地面の手前に物体を置く
		m_obj_move.Set_trans(Vector3(MoveObjectNode.GetTrans().x, 0.5f, MoveObjectNode.GetTrans().z));

		// 表示オブジェクトと同じ位置にあたり判定を設定
		MoveObjectNode.SetTrans(m_obj_move.Get_transmat());

		// ボールが地面をバウンドしている時
		if (BoundBallSpd < 0 && BoundBallSpd < -0.1f)
		{
			// 跳ね返す
			BoundBallSpd = -BoundBallSpd * 0.8f;
		}
		else
		{
			// バウンドがある程度なくなったらバウンドを止める
			BoundBallSpd = 0;
		}
	}

	// ボール処理
	{
		// ボールを移動
		Vector3 vec = MoveObjectNode.GetTrans();
		vec.y += BoundBallSpd;

		// 移動を反映
		m_obj_move.Set_trans(vec);

		MoveObjectNode.SetTrans(m_obj_move.Get_transmat());
	}


	Vector3* p;
	p = new Vector3;

	// 箱とプレイヤのあたり判定
	{
		Box _PlayerNode = m_player->GetBoxNode();
		Box _box = m_BNode;

		if (CheckBox2BoxAABB(_PlayerNode, _box, p))
		{
			// 上方向からの衝突処理
			if (_PlayerNode.Pos3.y <= _box.Pos0.y && _PlayerNode.Pos3.y > _box.Pos3.y - (_box.Pos3.y - _box.Pos0.y) / 2)
			{
				BoxNode& pN = m_player->GetBoxNode();
				m_player->SetTrans(Vector3(
					m_player->Get_transmat().x,
					_box.Pos0.y + (pN.GetSize().y) / 2.0f,
					m_player->Get_transmat().z));
				m_player->SetJump(0);
				m_player->JumpChange(true);
			}
			// 下方向からの衝突処理
			else if (_PlayerNode.Pos0.y >= _box.Pos3.y && _PlayerNode.Pos0.y < _box.Pos0.y - (_box.Pos0.y - _box.Pos3.y) / 2)
			{
				m_player->JumpChange(false);
				m_player->SetJump(0);
			}
		}
		m_Box.Update();
		m_BNode.Update();
	}

	// プレイヤと球のあたり判定
	{
		Box _box = m_player->GetBoxNode();
		Sphere _sphere = MoveObjectNode;

		if (CheckSphere2Box(_sphere, _box, p)) 
		{
			if (BoundBallSpd < 0) BoundBallSpd = -BoundBallSpd;
			BoundBallSpd += 0.1f;
		}
		m_obj_move.Update();
		MoveObjectNode.Update();
	}


	m_obj_skydome.Update();
	m_obj_ground.Update();

	m_player->Update();
	
	//地形モデルの読み込み
	for (int i = 0; i < wall; i++)
	{
		Box _PlayerNode = m_player->GetBoxNode();
		Box _box = m_groundBox[i];

		if (CheckBox2BoxAABB(_PlayerNode, _box, p))
		{
			m_player->StopMove();
			m_player->Colc();

		}
		m_obj_box[i].Update();
		m_groundBox[i].Update();
	}
}


void Floor1::Render()
{
	Draw& draw = Draw::GetInstance();

	//draw.RenderFont(L"Play");

	//毎フレーム描画処理
	DirectX::CommonStates m_states(draw.m_d3dDevice);
	draw.m_d3dContext->OMSetBlendState(m_states.Opaque(), nullptr, 0xFFFFFFFF);
	draw.m_d3dContext->OMSetDepthStencilState(m_states.DepthNone(), 0);
	draw.m_d3dContext->RSSetState(m_states.CullNone());

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);
	m_effect->SetWorld(m_world);
	m_effect->Apply(draw.m_d3dContext);
	draw.m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	////天球モデルの描画
	m_obj_skydome.Draw();

	////地面モデルの描画
	m_obj_ground.Draw();

	m_obj_move.Draw();
	MoveObjectNode.Render();


	//地形モデルの読み込み
	for (int i = 0; i < wall; i++)
	{
		m_obj_box[i].Draw();
		m_groundBox[i].Render();
	}

	m_player->Render();

	m_BNode.Render();

}

void Floor1::Dispose()
{
	if (m_base != NULL)
	{
		delete m_base;
	}
}

void Floor1::SceneChange(Manager * main)
{
	Key& key = Key::GetInstance();
	auto kb = key.m_keyboard->GetState();

	if (kb.D2)
	{
		main->Scene(Floor2::GetInstance());
	}
	if (kb.D3)
	{
		main->Scene(Floor3::GetInstance());
	}
	if (kb.D4)
	{
		main->Scene(Floor4::GetInstance());
	}
	if (kb.D5)
	{
		main->Scene(Floor5::GetInstance());
	}
	if (kb.D6)
	{
		main->Scene(Floor6::GetInstance());
	}
}

void Floor1::Map()
{
	// 各オブジェクトの配置の設定
	{
		m_obj_box[0].Set_trans(Vector3(5, 0, 0));
		m_obj_box[1].Set_trans(Vector3(5, 0, -1));
		m_obj_box[2].Set_trans(Vector3(5, 0, -2));
		m_obj_box[3].Set_trans(Vector3(5, 0, -3));
		m_obj_box[4].Set_trans(Vector3(5, 0, -4));
		m_obj_box[5].Set_trans(Vector3(5, 0, -5));
		m_obj_box[6].Set_trans(Vector3(5, 0, -6));
		m_obj_box[7].Set_trans(Vector3(5, 0, -7));
		m_obj_box[8].Set_trans(Vector3(5, 0, -8));
		m_obj_box[9].Set_trans(Vector3(5, 0, -9));
		m_obj_box[10].Set_trans(Vector3(5, 0, -10));

		m_obj_box[11].Set_trans(Vector3(-5, 0, 0));
		m_obj_box[12].Set_trans(Vector3(-5, 0, -1));
		m_obj_box[13].Set_trans(Vector3(-5, 0, -2));
		m_obj_box[14].Set_trans(Vector3(-5, 0, -3));
		m_obj_box[15].Set_trans(Vector3(-5, 0, -4));
		m_obj_box[16].Set_trans(Vector3(-5, 0, -5));
		m_obj_box[17].Set_trans(Vector3(-5, 0, -6));
		m_obj_box[18].Set_trans(Vector3(-5, 0, -7));
		m_obj_box[19].Set_trans(Vector3(-5, 0, -8));
		m_obj_box[20].Set_trans(Vector3(-5, 0, -9));
		m_obj_box[21].Set_trans(Vector3(-5, 0, -10));

		m_obj_box[22].Set_trans(Vector3(5, 0, -11));
		m_obj_box[24].Set_trans(Vector3(5, 0, -12));
		m_obj_box[25].Set_trans(Vector3(5, 0, -13));
		m_obj_box[26].Set_trans(Vector3(5, 0, -14));
		m_obj_box[27].Set_trans(Vector3(5, 0, -15));
		m_obj_box[28].Set_trans(Vector3(5, 0, -16));
		m_obj_box[29].Set_trans(Vector3(5, 0, -17));
		m_obj_box[30].Set_trans(Vector3(5, 0, -18));
		m_obj_box[31].Set_trans(Vector3(5, 0, -19));

		m_obj_box[32].Set_trans(Vector3(-5, 0, -11));
		m_obj_box[33].Set_trans(Vector3(-5, 0, -12));
		m_obj_box[23].Set_trans(Vector3(-5, 0, -13));
		m_obj_box[34].Set_trans(Vector3(-5, 0, -14));
		m_obj_box[35].Set_trans(Vector3(-5, 0, -15));
		m_obj_box[36].Set_trans(Vector3(-5, 0, -16));
		m_obj_box[37].Set_trans(Vector3(-5, 0, -17));
		m_obj_box[38].Set_trans(Vector3(-5, 0, -18));
		m_obj_box[39].Set_trans(Vector3(-5, 0, -19));

		m_obj_box[40].Set_trans(Vector3(-5, 0, -20));
		m_obj_box[41].Set_trans(Vector3(-5, 0, -21));
		m_obj_box[42].Set_trans(Vector3(5, 0, -20));
		m_obj_box[43].Set_trans(Vector3(5, 0, -21));
		m_obj_box[44].Set_trans(Vector3(5, 0, -22));

		m_obj_box[45].Set_trans(Vector3(-5, 0, -22));
		m_obj_box[46].Set_trans(Vector3(-4, 0, -22));
		m_obj_box[47].Set_trans(Vector3(-3, 0, -22));
		m_obj_box[48].Set_trans(Vector3(-2, 0, -22));
		m_obj_box[49].Set_trans(Vector3(-1, 0, -22));
		m_obj_box[50].Set_trans(Vector3(0, 0, -22));
		m_obj_box[51].Set_trans(Vector3(1, 0, -22));
		m_obj_box[52].Set_trans(Vector3(2, 0, -22));
		m_obj_box[53].Set_trans(Vector3(3, 0, -22));
		m_obj_box[54].Set_trans(Vector3(4, 0, -22));
		m_obj_box[55].Set_trans(Vector3(5, 0, -22));

		m_obj_box[56].Set_trans(Vector3(5, 0, 1));
		m_obj_box[57].Set_trans(Vector3(-5, 0, 1));

		m_obj_box[58].Set_trans(Vector3(-5, 0, 1));
		m_obj_box[59].Set_trans(Vector3(-4, 0, 1));
		m_obj_box[60].Set_trans(Vector3(-3, 0, 1));
		m_obj_box[61].Set_trans(Vector3(-2, 0, 1));
		m_obj_box[62].Set_trans(Vector3(-1, 0, 1));
		m_obj_box[63].Set_trans(Vector3(0, 0, 1));
		m_obj_box[64].Set_trans(Vector3(1, 0, 1));
		m_obj_box[65].Set_trans(Vector3(2, 0, 1));
		m_obj_box[66].Set_trans(Vector3(3, 0, 1));
		m_obj_box[67].Set_trans(Vector3(4, 0, 1));
		m_obj_box[68].Set_trans(Vector3(5, 0, 1)); 
	}

	for (int i = 0; i < wall; i++) {
		m_groundBox[i].SetTrans(m_obj_box[i].Get_transmat() + Vector3(0,0.5f,0));
	}
}

