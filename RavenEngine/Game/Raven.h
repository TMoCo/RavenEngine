
#include "Engine.h"

class RavenGame : public Raven::Engine
{
public:
	virtual void Initialize() override;
	void OnImGui() override;
};
