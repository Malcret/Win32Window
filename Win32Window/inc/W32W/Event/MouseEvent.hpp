#pragma once

#include "Event.hpp"

namespace W32W {
	enum class BUTTON;

	class MouseMovedEvent : public Event {
	public:
		_W32W_EVENT_CLASS_TYPE(MouseMoved)
		_W32W_EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		MouseMovedEvent(float xPos, float yPos)
			: m_xPos(xPos), m_yPos(yPos) {
		}

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_xPos << ", " << m_yPos;
			return ss.str();
		}

		inline float getXPos() const { return m_xPos; }
		inline float getYPos() const { return m_yPos; }

	private:
		float m_xPos, m_yPos;
	};

	class MouseScrolledEvent : public Event {
	public:
		_W32W_EVENT_CLASS_TYPE(MouseScrolled)
		_W32W_EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		MouseScrolledEvent(float offset)
			: m_offset(offset) {
		}

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_offset;
			return ss.str();
		}

		inline float getOffset() const { return m_offset; }

	private:
		float m_offset;
	};

	class MouseButtonEvent : public Event {
	public:
		_W32W_EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		inline int getButtonCode() const { return m_button; }
		inline BUTTON getButton() const { return (BUTTON)m_button; }

	protected:
		MouseButtonEvent(int button)
			: m_button(button) {
		}

		int m_button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		_W32W_EVENT_CLASS_TYPE(MouseButtonPressed)

		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {
		}

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_button;
			return ss.str();
		}
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		_W32W_EVENT_CLASS_TYPE(MouseButtonReleased)

		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {
		}

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_button;
			return ss.str();
		}
	};

	enum class BUTTON {
		LEFT     = 1,
		RIGHT    = 2,
		MIDDLE   = 3,
		BUTTON_4 = 4,
		BUTTON_5 = 5
	};
}