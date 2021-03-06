#pragma once




#include "IModule.h"




class RenderModule : public IModule
{
public:
	virtual void Load();
	virtual void Update();
	virtual void Destroy();

	static EModuleType GetType()
	{
		return MT_Render;
	}

};




