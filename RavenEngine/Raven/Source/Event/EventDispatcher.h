//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <cstdint>
#include <queue>
#include <functional>
#include <set>
#include <future>
#include "Event.h"
#include "EventHandler.h"
#include "Utilities/Core.h"

namespace Raven 
{
	class EventDispatcher final
	{
	public:
		EventDispatcher();
		~EventDispatcher();

		void AddEventHandler(EventHandler* handler);
		void RemoveEventHandler(EventHandler* handler);
		bool DispatchEvent(std::unique_ptr<Event>&& event);
		std::future<bool> PostEvent(std::unique_ptr<Event>&& event);
		void DispatchEvents();

	private:
		std::vector<EventHandler *> eventHandlers;
		std::set<EventHandler *> eventHandlerAddSet;
		std::set<EventHandler *> eventHandlerDeleteSet;

		std::mutex eventQueueMutex;
		std::queue<std::pair<std::promise<bool>, std::unique_ptr<Event>>> eventQueue;
	};

};

