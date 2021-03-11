//

#pragma once


#include "IModule.h"


#include <array>
#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>
#include <functional>



/**
 * Engine:
 *		- 
 */

namespace Raven 
{

	class Scene;
	class Engine
	{
	public:
		/** Constructor. */
		Engine();

		/** Destructor. */
		~Engine();

		/** Return the Engine Singleton. */
		static Engine& Get();

		/** Initialize the Engine. */
		virtual void Initialize();

		/** Run the Engine. */
		int Run();


		/** Static function returns the Module of type Module Type. */
		template<class TModule>
		static inline TModule* GetModule()
		{
			return static_cast<TModule*>(Get().engineModules[TModule::GetModuleType()]);
		}


		//MainThread Operation
		std::future<bool> Post(const std::function<bool()>& callback);

		virtual void OnSceneCreated(Scene* scene);

	protected:
		virtual void OnImGui();
		virtual void OnUpdate(float dt);
		virtual void OnRender();
	private:
		/** Create a the module and store it in the engine modules list. */
		template<class TModule, typename...Args>
		inline void CreateModule(Args&&... args)
		{
			engineModules[TModule::GetModuleType()] = new TModule(std::forward<Args>(args)...);
		}

		/** Initialize a module. */
		template<class TModule>
		inline void InitializeModule()
		{
			engineModules[TModule::GetModuleType()]->Initialize();
		}

		/** Destroy a module. */
		template<class TModule>
		inline void DestroyModule()
		{
			engineModules[TModule::GetModuleType()]->Destroy();
		}

		/** Load required modules. */
		void LoadModules();

		/** Destory all loaded modules. */
		void DestoryModules();

	


	private:

		//main thread --call Post to post callback to main-thread if you are in other threads.
		std::queue<std::pair<std::promise<bool>, std::function<bool()>>> executeQueue;
		std::mutex executeMutex;

		/** List of all the modules in the engine. */
		std::array<IModule*, MT_MAX> engineModules;
	};
};
Raven::Engine* CreateEngine();

