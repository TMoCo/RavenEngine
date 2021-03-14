
#include "Engine.h"

class RavenGame : public Engine
{
public:
	virtual void Initialize() override;
	void OnImGui() override;
};
