#pragma once


#include <string>
#include <queue>
#include <memory>
#include "Logger/Console.h"
#include "Utilities/Core.h"
#include "IModule.h"



namespace Raven
{
	class Scene;


	// SceneManager:
	//    - 
	class SceneManager final : public IModule
	{
	public:
		// Construct.
		SceneManager();

		// Destruct.
		~SceneManager();

		// Set a new scene to switch to the next time we update.
		void SwitchToScene(const std::string& name);
		void SwitchToScene(uint32_t index);

		// Apply any pending changes.
		void Apply();

		inline Scene* GetCurrentScene() const { return currentScene; }
		inline int32_t GetCurrentIndex() const { return currentSceneIndex; }

		//...
		inline int32_t GetSceneCount() const { return static_cast<int32_t>(scenes.size()); }
		inline auto& GetScenes() const { return scenes; }
		std::vector<std::string> GetSceneNames();
		inline auto& GetSceneFilePaths() const { return sceneFilePaths; }

		// Return true if we have pending scene switch.
		inline auto IsSwitchingScene() const  { return switchingScenes; }

		// Create a new scene and load it form file.
		void AddSceneFromFile(const std::string& filePath);
		
		// Get Scene.
		int32_t GetSceneIndex(Scene* inScene);
		int32_t GetSceneIndex(const std::string& sceneName);
		Scene* GetSceneByName(const std::string& sceneName);
		Scene* GetScene(int32_t index);

		void AddFileToLoadList(const std::string& filePath);

		void LoadCurrentList();

		// Add new scene to be managed, the scene pointer will be managed by this class.
		int32_t AddScene(Scene* scene);

		// Create/Add new scene to the manager.
		template<class TScene>
		Ptr<TScene> AddScene(const std::string& name);

		// Remove a scene with this name.
		void RemoveScene(const std::string& name);

		// ~IModule
		virtual void Initialize() override;
		virtual void Destroy() override;
		static auto GetModuleType() { return MT_SceneManager; }

	private:
		// Set switch flag.
		inline auto SetSwitchScene(bool switching) { switchingScenes = switching; }

	protected:
		// The index of the current active scene.
		int32_t currentSceneIndex = 0;

		// The Current active scene.
		Scene* currentScene = nullptr;

		// all scenes in the Engine.
		std::vector< Ptr<Scene> > scenes;

		// ??
		std::vector<std::string> sceneFilePaths;
		std::vector<std::string> sceneFilePathsToLoad;

	private:
		bool switchingScenes = false;
		int32_t queuedSceneIndex = -1;
		NOCOPYABLE(SceneManager);
	};


	template<class TScene>
	Ptr<TScene> SceneManager::AddScene(const std::string& name)
	{
		RAVEN_ASSERT(GetSceneIndex(name) == -1, "Scene name already exist.");

		LOGI("[{0}] - Enqueued scene : {1}", __FILE__, name.c_str());
		Ptr<Scene> newScene = scenes.emplace_back( std::make_shared<TScene>(name) );
		return std::static_pointer_cast<TScene, Scene>(newScene);
	}

}
