#pragma once

#include "Event.hpp"

namespace W32W {
	enum class KEY;

	class KeyEvent : public Event {
	public:
		_W32W_EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

		inline int getKeyCode() const { return m_keyCode; }
		inline KEY getKey() const { return (KEY)m_keyCode; }

	protected:
		KeyEvent(int keyCode)
			: m_keyCode(keyCode) {
		}

		int m_keyCode;
	};

	class KeyPressEvent : public KeyEvent {
	public:
		_W32W_EVENT_CLASS_TYPE(KeyPress)

		KeyPressEvent(int keyCode, unsigned int repeatCount)
			: KeyEvent(keyCode), m_repeatCount(repeatCount) {
		}

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "KeyPressEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}

		inline unsigned int getRepeatCount() const { return m_repeatCount; }

	private:
		unsigned int m_repeatCount;
	};

	class KeyReleaseEvent : public KeyEvent {
	public:
		_W32W_EVENT_CLASS_TYPE(KeyRelease)

		KeyReleaseEvent(int keyCode)
			: KeyEvent(keyCode) {
		}

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << m_keyCode;
			return ss.str();
		}
	};

	enum class KEY {
		BACKSPACE           = 0x08,
		TAB                 = 0x09,
		CLEAR               = 0x0C,
		RETURN              = 0x0D,
		SHIFT               = 0x10,
		CTRL                = 0x11,
		ALT                 = 0x12,
		PAUSE               = 0x13,
		CAPS_LOCK           = 0x14,
		ESCAPE              = 0x1B,
		SPACE               = 0x20,
		PAGE_UP             = 0x21,
		PAGE_DOWN           = 0x22,
		END                 = 0x23,
		HOME                = 0x24,
		LEFT                = 0x25,
		UP                  = 0x26,
		RIGHT               = 0x27,
		DOWN                = 0x28,
		SELECT              = 0x29,
		PRINT               = 0x2A,
		EXECUTE             = 0x2B,
		PRINT_SCREEN        = 0x2C,
		INSERT              = 0x2D,
		DELETE_KEY          = 0x2E,
		HELP                = 0x2F,
		KEY_0               = 0x30,
		KEY_1               = 0x31,
		KEY_2               = 0x32,
		KEY_3               = 0x33,
		KEY_4               = 0x34,
		KEY_5               = 0x35,
		KEY_6               = 0x36,
		KEY_7               = 0x37,
		KEY_8               = 0x38,
		KEY_9               = 0x39,
		A                   = 0x41,
		B                   = 0x42,
		C                   = 0x43,
		D                   = 0x44,
		E                   = 0x45,
		F                   = 0x46,
		G                   = 0x47,
		H                   = 0x48,
		I                   = 0x49,
		J                   = 0x4A,
		K                   = 0x4B,
		L                   = 0x4C,
		M                   = 0x4D,
		N                   = 0x4E,
		O                   = 0x4F,
		P                   = 0x50,
		Q                   = 0x51,
		R                   = 0x52,
		S                   = 0x53,
		T                   = 0x54,
		U                   = 0x55,
		V                   = 0x56,
		W                   = 0x57,
		X                   = 0x58,
		Y                   = 0x59,
		Z                   = 0x5A,
		LWIN                = 0x5B,
		RWIN                = 0x5C,
		APPS                = 0x5D,
		SLEEP               = 0x5F,
		NUMPAD_0            = 0x60,
		NUMPAD_1            = 0x61,
		NUMPAD_2            = 0x62,
		NUMPAD_3            = 0x63,
		NUMPAD_4            = 0x64,
		NUMPAD_5            = 0x65,
		NUMPAD_6            = 0x66,
		NUMPAD_7            = 0x67,
		NUMPAD_8            = 0x68,
		NUMPAD_9            = 0x69,
		MULTIPLY            = 0x6A,
		ADD                 = 0x6B,
		SEPARATOR           = 0x6C,
		SUBTRACT            = 0x6D,
		DECIMAL             = 0x6E,
		DIVIDE              = 0x6F,
		F1                  = 0x70,
		F2                  = 0x71,
		F3                  = 0x72,
		F4                  = 0x73,
		F5                  = 0x74,
		F6                  = 0x75,
		F7                  = 0x76,
		F8                  = 0x77,
		F9                  = 0x78,
		F10                 = 0x79,
		F11                 = 0x7A,
		F12                 = 0x7B,
		F13                 = 0x7C,
		F14                 = 0x7D,
		F15                 = 0x7E,
		F16                 = 0x7F,
		F17                 = 0x80,
		F18                 = 0x81,
		F19                 = 0x82,
		F20                 = 0x83,
		F21                 = 0x84,
		F22                 = 0x85,
		F23                 = 0x86,
		F24                 = 0x87,
		NUMLOCK             = 0x90,
		SCROLL              = 0x91,
		L_SHIFT             = 0xA0,
		R_SHIFT             = 0xA1,
		L_CONTROL           = 0xA2,
		R_CONTROL           = 0xA3,
		L_ALT               = 0xA4,
		R_ALT               = 0xA5,
		BROWSER_BACK        = 0xA6,
		BROWSER_FORWARD     = 0xA7,
		BROWSER_REFRESH     = 0xA8,
		BROWSER_STOP        = 0xA9,
		BROWSER_SEARCH      = 0xAA,
		BROWSER_FAVORITES   = 0xAB,
		BROWSER_HOME        = 0xAC,
		VOLUME_MUTE         = 0xAD,
		VOLUME_DOWN         = 0xAE,
		VOLUME_UP           = 0xAF,
		MEDIA_NEXT_TRACK    = 0xB0,
		MEDIA_PREV_TRACK    = 0xB1,
		MEDIA_STOP          = 0xB2,
		MEDIA_PLAY_PAUSE    = 0xB3,
		LAUNCH_MAIL         = 0xB4,
		LAUNCH_MEDIA_SELECT = 0xB5,
		LAUNCH_APP1         = 0xB6,
		LAUNCH_APP2         = 0xB7,
		OEM_1               = 0xBA,
		OEM_PLUS            = 0xBB,
		OEM_COMMA           = 0xBC,
		OEM_MINUS           = 0xBD,
		OEM_PERIOD          = 0xBE,
		OEM_2               = 0xBF,
		OEM_3               = 0xC0,
		OEM_4               = 0xDB,
		OEM_5               = 0xDC,
		OEM_6               = 0xDD,
		OEM_7               = 0xDE,
		OEM_8               = 0xDF,
		OEM_102             = 0xE2,
		PROCESSKEY          = 0xE5,
		PACKET              = 0xE7,
		ATTN                = 0xF6,
		CRSEL               = 0xF7,
		EXSEL               = 0xF8,
		EREOF               = 0xF9,
		PLAY                = 0xFA,
		ZOOM                = 0xFB,
		PA1                 = 0xFD,
		OEM_CLEAR           = 0xFE,
		KEY_COUNT           = 0xFF,
	};
}