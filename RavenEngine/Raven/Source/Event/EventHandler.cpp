//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "EventHandler.h"
#include "EventDispatcher.h"


namespace Raven 
{
	EventHandler::~EventHandler()
	{
		if (eventDispatcher)
			eventDispatcher->RemoveEventHandler(this);
	}

	auto EventHandler::Remove() -> void
	{
		if (eventDispatcher)
		{
			eventDispatcher->RemoveEventHandler(this);
			eventDispatcher = nullptr;
		}
	}
};
