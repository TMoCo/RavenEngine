//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AudioExport.h"
#include "Scripts/LuaVirtualMachine.h"
#include <LuaBridge/LuaBridge.h>
#include "Audio/AudioSource.h"

namespace Raven
{
	namespace AudioExport
	{
		void Export(lua_State* L) 
		{
			luabridge::getGlobalNamespace(L)
				.beginClass<AudioSource>("AudioSource")
				.addStaticFunction("new", &AudioSource::CreateLuaObj)
				.addFunction("SetAudioClip", &AudioSource::SetAudioClip)
				.addFunction("Play", &AudioSource::Resume)
				.addFunction("Resume", &AudioSource::Resume)
				.addFunction("Stop", &AudioSource::Stop)
				.addFunction("Pause", &AudioSource::Pause)
				.addFunction("SetLooping", &AudioSource::SetLooping)
				.endClass()

				.beginClass<AudioClip>("AudioClip")
				.addStaticFunction("new", &AudioClip::CreateLuaObj)
				.endClass();
				
		}
	};
};