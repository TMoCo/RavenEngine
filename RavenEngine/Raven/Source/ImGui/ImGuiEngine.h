//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
// Copyright ?2021-2023 Prime Zeng                                          //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "IModule.h"
namespace Raven 
{
	class ImGuiEngine : public IModule
	{
	public:
		virtual void Initialize() override;
		virtual void Destroy() override;
		virtual void Render();
		virtual void Prepare();
		static EModuleType GetModuleType() { return MT_ImGui; }

		void AddIcon();

	};
};

