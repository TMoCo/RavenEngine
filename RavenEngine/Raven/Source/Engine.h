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


		inline auto GetEditorState() const { return state; }
		inline auto SetEditorState(EditorState state) { this->state = state; }

		inline auto& GetEventDispatcher() { return eventDispatcher; }
		
		
		inline void SetSceneActive(bool active){sceneActive = active;}
		//mainly used in editor mode. when the scene is selected, return true
		inline bool IsSceneActive() const {return sceneActive;}

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
			delete engineModules[ TModule::GetModuleType() ];
		}

		/** Load required modules. */
		void LoadModules();

		/** Destory all loaded modules. */
		void DestoryModules();

	


	protected:

		//main thread --call Post to post callback to main-thread if you are in other threads.
		std::queue<std::pair<std::promise<bool>, std::function<bool()>>> executeQueue;
		std::mutex executeMutex;

		/** List of all the modules in the engine. */
		std::array<IModule*, MT_MAX> engineModules;

		EditorState state = EditorState::Play; //default as Play, if editor is loaded, the state will be set as preview.

		EventDispatcher eventDispatcher;


		bool sceneActive = false;

	};
};
Raven::Engine* CreateEngine();

