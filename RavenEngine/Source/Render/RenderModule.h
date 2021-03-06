#pragma once




#include "IModule.h"




/**
 *
 */
class RenderModule : public IModule
{
public:
	/** Construct. */
	RenderModule();

	/** Destruct. */
	~RenderModule();

	/** Return the type of the module. */
	static EModuleType GetModuleType() { return MT_Render; }


	/** */
	inline void Example() { }

private:
	/** Module Initialize. */
	virtual void Initialize() override;

	/** Module Destroy. */
	virtual void Destroy() override;

	
public:
	/** Update the render. */
	void Update();

};


