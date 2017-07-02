#include "stdafx.h"
#include "myEngine/HID/Pad.h"


#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.

namespace {
	/*!
	*@brief	���z�{�^����XBox�R���g���[���̃{�^���Ƃ̊֘A�t����\���\���́B
	*/
	struct VirtualPadToXPad {
		Pad::EnButton vButton;		//!<���z�{�^���B
		DWORD		  xButton;		//!<XBox�R���g���[���̃{�^���B
	};

	//���z�{�^����XBox�R���g���[���Ƃ̃{�^���̕ϊ��e�[�u���B
	const VirtualPadToXPad vPadToXPadTable[Pad::enButtonNum] = {
		{ Pad::enButtonUp		, XINPUT_GAMEPAD_DPAD_UP },
		{ Pad::enButtonDown		, XINPUT_GAMEPAD_DPAD_DOWN },
		{ Pad::enButtonLeft		, XINPUT_GAMEPAD_DPAD_LEFT },
		{ Pad::enButtonRight	, XINPUT_GAMEPAD_DPAD_RIGHT },
		{ Pad::enButtonA		, XINPUT_GAMEPAD_A },
		{ Pad::enButtonB		, XINPUT_GAMEPAD_B },
		{ Pad::enButtonY		, XINPUT_GAMEPAD_Y },
		{ Pad::enButtonX		, XINPUT_GAMEPAD_X },
		{ Pad::enButtonSelect	, XINPUT_GAMEPAD_BACK },
		{ Pad::enButtonStart	, XINPUT_GAMEPAD_START },
		{ Pad::enButtonRB1		, XINPUT_GAMEPAD_RIGHT_SHOULDER },
		{ Pad::enButtonRB2		, 0 },
		{ Pad::enButtonRB3		, XINPUT_GAMEPAD_RIGHT_THUMB },
		{ Pad::enButtonLB1		, XINPUT_GAMEPAD_LEFT_SHOULDER },
		{ Pad::enButtonLB2		, 0 },
		{ Pad::enButtonLB3		, XINPUT_GAMEPAD_LEFT_THUMB },
	};
}
Pad::Pad() :
	m_padNo(0)
{
	memset(&m_state, 0, sizeof(m_state));
	memset(m_trigger, 0, sizeof(m_trigger));
	memset(m_press, 0, sizeof(m_press));
	m_lStickX = 0.0f;
	m_lStickY = 0.0f;
	m_rStickX = 0.0f;
	m_rStickY = 0.0f;
}
Pad::~Pad()
{
}
void Pad::Update()
{
	DWORD result = XInputGetState(m_padNo, &m_state);
	if (result == ERROR_SUCCESS) {
		//�ڑ�����Ă���B
		
		for (const VirtualPadToXPad& vPadToXPad : vPadToXPadTable) {
			if (m_state.Gamepad.wButtons & vPadToXPad.xButton) {
				m_trigger[vPadToXPad.vButton] = 1 ^ m_press[vPadToXPad.vButton];
				m_press[vPadToXPad.vButton] = 1;
			}
			else {
				m_trigger[vPadToXPad.vButton] = 0;
				m_press[vPadToXPad.vButton] = 0;
			}
		}
		//���g���K�[�B
		if (m_state.Gamepad.bLeftTrigger != 0) {
			m_trigger[enButtonLB2] = 1 ^ m_press[enButtonLB2];
			m_press[enButtonLB2] = 1;
		}
		else {
			m_trigger[enButtonLB2] = 0;
			m_press[enButtonLB2] = 0;
		}
		//�E�g���K�[
		if (m_state.Gamepad.bRightTrigger != 0) {
			m_trigger[enButtonRB2] = 1 ^ m_press[enButtonRB2];
			m_press[enButtonRB2] = 1;
		}
		else {
			m_trigger[enButtonRB2] = 0;
			m_press[enButtonRB2] = 0;
		}
		if ((m_state.Gamepad.sThumbLX < INPUT_DEADZONE &&
			m_state.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
			(m_state.Gamepad.sThumbLY < INPUT_DEADZONE &&
				m_state.Gamepad.sThumbLY > -INPUT_DEADZONE))
		{
			m_state.Gamepad.sThumbLX = 0;
			m_state.Gamepad.sThumbLY = 0;
			m_lStickX = 0.0f;
			m_lStickY = 0.0f;
		}
		else {
			//���X�e�B�b�N�̓��͗ʁB
			if (m_state.Gamepad.sThumbLX > 0) {
				m_lStickX = (float)(m_state.Gamepad.sThumbLX) / SHRT_MAX;
			}
			else {
				m_lStickX = (float)(m_state.Gamepad.sThumbLX) / -SHRT_MIN;
			}
			if (m_state.Gamepad.sThumbLY > 0) {
				m_lStickY = (float)(m_state.Gamepad.sThumbLY) / SHRT_MAX;
			}
			else {
				m_lStickY = (float)(m_state.Gamepad.sThumbLY) / -SHRT_MIN;
			}
		}

		if ((m_state.Gamepad.sThumbRX < INPUT_DEADZONE &&
			m_state.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
			(m_state.Gamepad.sThumbRY < INPUT_DEADZONE &&
				m_state.Gamepad.sThumbRY > -INPUT_DEADZONE))
		{
			m_state.Gamepad.sThumbRX = 0;
			m_state.Gamepad.sThumbRY = 0;
			m_rStickX = 0.0f;
			m_rStickY = 0.0f;
		}
		else {
			//�E�X�e�B�b�N�̓��͗ʁB
			if (m_state.Gamepad.sThumbRX > 0) {
				m_rStickX = (float)(m_state.Gamepad.sThumbRX) / SHRT_MAX;
			}
			else {
				m_rStickX = (float)(m_state.Gamepad.sThumbRX) / -SHRT_MIN;
			}
			if (m_state.Gamepad.sThumbRY > 0) {
				m_rStickY = (float)(m_state.Gamepad.sThumbRY) / SHRT_MAX;
			}
			else {
				m_rStickY = (float)(m_state.Gamepad.sThumbRY) / -SHRT_MIN;
			}
		}
	}
}