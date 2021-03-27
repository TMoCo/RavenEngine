//

#pragma once


#include "IModule.h"


#include <array>
#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>
#include <functional>

#include "Event/EventDispatcher.h"

/**
 * Engine:
 *		- 
 */

namespace Raven 
{

	class SystemManager;

	enum class EditorState
	{
		Paused,
		Play,
		Next,
		Preview
	};

	class Scene;

	class Engine
	{
	public:
		// Constructor
		Engine();

		// Destructor
		~Engine();

		// Return engine singleton instance
		static Engine& Get();

		// Initialise the engine
		virtual void Initialize();

		// Function for running engine
		int Run();

		// Return a pointer to a module of type TModule from std::array of modules
		template<class TModule>
		static inline TModule* GetModule()
		{
			return static_cast<TModule*>(Get().engineModules[TModule::GetModuleType()]);
		}

		// MainThread Operation
		std::future<bool> Post(const std::function<bool()>& callback);

		virtual void OnSceneCreated(Scene* scene);

		inline auto GetEditorState() const { return state; }
		inline auto SetEditorState(EditorState state) { this->state = state; }

		inline auto& GetEventDispatcher() { return eventDispatcher; }
		
		
		inline void SetSceneActive(bool active){sceneActive = active;}
		//mainly used in editor mode. when the scene is selected, return true
		inline bool IsSceneActive() const {return sceneActive;}

		inline auto& GetSystemManager() { return systemManager; }

	protected:
		virtual void OnImGui();
		virtual void OnUpdate(float dt);
		virtual void OnRender();
	private:
		// Create a module and store in array of modules, pass aditional arguments to module constructor
		template<class TModule, typename...Args>
		inline void CreateModule(Args&&... args)
		{
			engineModules[TModule::GetModuleType()] = new TModule(std::forward<Args>(args)...);
		}

		// Initialise a module
		template<class TModule>
		inline void InitializeModule()
		{
			engineModules[TModule::GetModuleType()]->Initialize();
		}

		// Destroy a module
		template<class TModule>
		inline void DestroyModule()
		{
			engineModules[TModule::GetModuleType()]->Destroy();
			delete engineModules[ TModule::GetModuleType() ];
		}

		// Load all modules 
		void LoadModules();

		// Destroy all loaded modules
		void DestoryModules();	


	protected:

		// Main thread -- call Post to post callback to main-thread if you are in other threads.
		std::queue<std::pair<std::promise<bool>, std::function<bool()>>> executeQueue;
		std::mutex executeMutex;

		// Modules in the engine
		std::array<IModule*, MT_MAX> engineModules;

		EditorState state = EditorState::Play; //default as Play, if editor is loaded, the state will be set as preview.

		EventDispatcher eventDispatcher;

		std::unique_ptr<SystemManager> systemManager;

		bool sceneActive = false;

	};
};

Raven::Engine* CreateEngine();

