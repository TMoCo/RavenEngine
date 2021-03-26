
#include "Engine.h"

namespace Raven
{
	class RavenGame : public Engine
	{
	public:
		virtual void Initialize() override;
		void OnImGui() override;
		void OnUpdate(float dt) override;
	};
}

