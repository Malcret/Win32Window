#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include "MWW/Utility.hpp"

#define MWW_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace MWW {
	enum class EventType {
		None = 0,
		WindowClose, WindowMinimize, WindowMaximize, WindowResize, WindowRestore, WindowMove, WindowFocus, WindowLostFocus,
		KeyPress, KeyRelease,
		MouseMove, MouseScroll, MouseButtonPress, MouseButtonRelease
	};

	enum EventCategoryFlag {
		None = 0,
		EventCategoryWindow = MWW_BIT(0),
		EventCategoryInput = MWW_BIT(1),
		EventCategoryKeyboard = MWW_BIT(2),
		EventCategoryMouse = MWW_BIT(3),
		EventCategoryMouseButton = MWW_BIT(4)
	};

	#define _MWW_EVENT_CLASS_TYPE(type) inline static EventType getStaticEventType() { return EventType::type; } \
	                                     inline EventType getEventType() const { return getStaticEventType(); } \
	                                     inline const char *getName() const { return #type; }

	#define _MWW_EVENT_CLASS_CATEGORY(category) virtual inline unsigned short getCategoryFlags() const override { return category; }

	class Event {
		template<typename T>
		using EventFunction = std::function<bool(T &)>;

	public:
		virtual inline EventType getEventType() const = 0;
		virtual inline unsigned short getCategoryFlags() const = 0;
		virtual inline const char *getName() const = 0;
		virtual inline std::string toString() const { return getName(); }
		inline bool isInCategory(const EventCategoryFlag category) const { return getCategoryFlags() & category; }

		template<typename T>
		inline static bool dispatch(Event &event, EventFunction<T> function) {

			if (event.getEventType() == T::getStaticEventType()) {
				event.m_handled = function(*(T *)&event);
				return true;
			}
			return false;
		}

	protected:
		bool m_handled = false;
	};

	inline std::ostream &operator<<(std::ostream &os, const Event &e) {
		return os << e.toString();
	}

	using EventCallbackFunc = std::function<void(Event &e)>;
}