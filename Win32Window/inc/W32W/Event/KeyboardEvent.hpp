#pragma once

#include "Event.hpp"

namespace W32W {
	class KeyEvent : public Event {
	public:
		_W32W_EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

		inline int getKeyCode() const { return m_keyCode; }

	protected:
		KeyEvent(int keyCode)
			: m_keyCode(keyCode) {
		}

		int m_keyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		_W32W_EVENT_CLASS_TYPE(KeyPressed)

		KeyPressedEvent(int keyCode, unsigned int repeatCount)
			: KeyEvent(keyCode), m_repeatCount(repeatCount) {
		}

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}

		inline unsigned int getRepeatCount() const { return m_repeatCount; }

	private:
		unsigned int m_repeatCount;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		_W32W_EVENT_CLASS_TYPE(KeyReleased)

		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) {
		}

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_keyCode;
			return ss.str();
		}
	};
}