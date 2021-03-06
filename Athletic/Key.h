//----------------------------------------------------
//*t@C¼FKey.h									
//*àe@@@FL[{[h							
//*§ìÒ@@FAyaka Yamanaka							
//----------------------------------------------------

#pragma once
#include <Keyboard.h>

class Key
{
public:
	// L[{[hÖW
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyTracker;

	static Key& GetInstance();
	void Update();

private:
	Key();
};