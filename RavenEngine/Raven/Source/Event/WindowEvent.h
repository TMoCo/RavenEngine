//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Event.h"

namespace Raven
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int32_t width, int32_t height);
		inline auto GetWidth() const { return width; }
		inline auto GetHeight() const { return height; }
		GENERATE_EVENT_CLASS_TYPE(WindowResize);
	private:
		int32_t width,height;
	};
};