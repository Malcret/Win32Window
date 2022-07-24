#pragma once

#include "Event.hpp"

namespace MWW {
	class WindowCloseEvent : public Event {
	public:
		_MWW_EVENT_CLASS_TYPE(WindowClose)
		_MWW_EVENT_CLASS_CATEGORY(EventCategoryWindow)

		WindowCloseEvent() {}
	};

	class WindowMinimizeEvent : public Event {
	public:
		_MWW_EVENT_CLASS_TYPE(WindowMinimize)
		_MWW_EVENT_CLASS_CATEGORY(EventCategoryWindow)

		WindowMinimizeEvent() {}
	};

	class WindowMaximizeEvent : public Event {
	public:
		_MWW_EVENT_CLASS_TYPE(WindowMaximize)
		_MWW_EVENT_CLASS_CATEGORY(EventCategoryWindow)

		WindowMaximizeEvent() {}
	};

	class WindowRestoreEvent : public Event {
	public:
		_MWW_EVENT_CLASS_TYPE(WindowRestore)
		_MWW_EVENT_CLASS_CATEGORY(EventCategoryWindow)

		WindowRestoreEvent(EventType oldWndState)
			: m_oldWndState(oldWndState) {}

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "WindowRestoreEvent: ";
			if (wasMinimized()) {
				ss << "Minimize";
			}
			else if (wasMaximized()) {
				ss << "Maximize";
			}
			else {
				ss << "Unknown";
			}
			return ss.str();
		}

		inline bool wasMinimized() const { return m_oldWndState == EventType::WindowMinimize; }
		inline bool wasMaximized() const { return m_oldWndState == EventType::WindowMaximize; }

	private:
		EventType m_oldWndState;
	};

	class WindowResizeEvent : public Event {
	public:
		_MWW_EVENT_CLASS_TYPE(WindowResize)
		_MWW_EVENT_CLASS_CATEGORY(EventCategoryWindow)

		WindowResizeEvent(int width, int height)
			: m_width(width), m_height(height) {
		}

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_width << ", " << m_height;
			return ss.str();
		}

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }

	private:
		unsigned int m_width, m_height;
	};

	class WindowMoveEvent : public Event {
	public:
		_MWW_EVENT_CLASS_TYPE(WindowMove)
		_MWW_EVENT_CLASS_CATEGORY(EventCategoryWindow)

		WindowMoveEvent(int xPos, int yPos)
			: m_xPos(xPos), m_yPos(yPos) {
		}

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "WindowMoveEvent: " << m_xPos << ", " << m_yPos;
			return ss.str();
		}

		inline int getXPos() const { return m_xPos; }
		inline int getYPos() const { return m_yPos; }

	private:
		unsigned int m_xPos, m_yPos;
	};

	class WindowFocusEvent : public Event {
	public:
		_MWW_EVENT_CLASS_TYPE(WindowFocus)
		_MWW_EVENT_CLASS_CATEGORY(EventCategoryWindow)

		WindowFocusEvent() {}
	};

	class WindowLostFocusEvent : public Event {
	public:
		_MWW_EVENT_CLASS_TYPE(WindowLostFocus)
		_MWW_EVENT_CLASS_CATEGORY(EventCategoryWindow)

		WindowLostFocusEvent() {}
	};
}