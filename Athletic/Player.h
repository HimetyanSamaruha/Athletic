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
	// 1フレームでの角度変化制限<度→ラジアン>
	static const float ANGLE_DELTA_MAX;

	//自機パーツ
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

	//スケーリング（XYZ)
	DirectX::SimpleMath::Vector3 Get_scale();
	//回転角(XYZ）
	float Get_rotate();
	//平行移動（XYZ）
	DirectX::SimpleMath::Vector3 Get_transmat();

	DirectX::SimpleMath::Matrix Get_world();



private:
	bool m_attack;			//攻撃フラグ
	int m_attack_cnt;		//攻撃カウント

	bool m_defense;			//防御フラグ
	int m_defense_cnt;		//防御カウント

	bool m_jump;			//ジャンプフラグ
	int m_jump_cnt;			//ジャンプカウント
	float jumping;			//方向ベクトルの初期化
	DirectX::SimpleMath::Vector3 vec;

	bool collision;			//当たり判定の表示

	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;

	std::vector<Obj3d> m_ObjPlayer;

	//キーボード
	DirectX::Keyboard* keyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> keyTracker;


	//弾丸の速度ベクトル
	DirectX::SimpleMath::Vector3 m_BulletVel;


};

