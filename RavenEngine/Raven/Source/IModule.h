#pragma once



#include <vector>





enum EModuleType
{
	MT_Render,

	MT_MAX
};





class IModule
{
private:
	friend class Engine;

	virtual void Load() = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;

};

