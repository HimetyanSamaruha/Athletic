#pragma once

#include "Camera.h"
#include <Keyboard.h>
#include "Player.h"

class FollowCamera :public Camera
{
private:
	static const float CAMERA_DISTANCE;

protected:
	// �Ǐ]�ΏۑΏۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	// �Ǐ]�Ώۂ̉�]�p
	float m_targetAngle;

	// �L�[�{�[�h
	DirectX::Keyboard* m_keyboard;
	// �L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;

	// �J�����؂�ւ��t���O
	bool m_isChangeFPS;

	//�v���C���[
	Player* m_player;

	DirectX::SimpleMath::Vector3 m_target_pos;
	float m_target_rotate;

public:

	FollowCamera(int width, int height);

	virtual ~FollowCamera();

	void Update();

	// �Ǐ]�Ώۂ̍��W���Z�b�g
	void SetTargetPos(DirectX::SimpleMath::Vector3 targetPos);

	// �Ǐ]�Ώۂ̉�]�p���Z�b�g
	void SetTargetAngle(float targetAngle);

	// �L�[�{�[�h���Z�b�g
	void SetKeyboard(DirectX::Keyboard* keyboard);

	// �v���C���[���Z�b�g
	void SetPlayer(Player* player) { m_player = player; }

};