//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
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

	class SceneManager final : public IModule
	{
	public:
		SceneManager() = default;
		void SwitchScene();
		void SwitchScene(const std::string& name);
		void SwitchScene(uint32_t index);

		void Apply();

		inline auto GetCurrentScene() const { return currentScene; }

		inline auto GetCurrentIndex() const { return sceneIdx; }

		inline auto GetSceneCount() const { return static_cast<uint32_t>(allScenes.size()); }

		inline auto& GetScenes() const { return allScenes; }


		inline auto & GetSceneFilePaths() const { return sceneFilePaths; }

		std::vector<std::string> GetSceneNames();

		inline auto SetSwitchScene(bool switching){ switchingScenes = switching; }

		inline auto IsSwitchingScene() const  { return switchingScenes; }

		void AddSceneFromFile(const std::string& filePath);
		void AddScene(Scene* scene);
		
		Scene* GetSceneByName(const std::string& sceneName);

		template<class T>
		void AddScene(const std::string& name);

		void AddFileToLoadList(const std::string& filePath);

		void LoadCurrentList();

		virtual void Initialize() override;

		virtual void Destroy() override;

		static auto GetModuleType() { return MT_SceneManager; }

	protected:
		uint32_t sceneIdx = -1;
		Scene* currentScene = nullptr;
		std::vector<std::shared_ptr<Scene>> allScenes;
		std::vector<std::string> sceneFilePaths;
		std::vector<std::string> sceneFilePathsToLoad;

	private:
		bool switchingScenes = false;
		int32_t queuedSceneIndex = -1;
		NOCOPYABLE(SceneManager);
	};

	template<class T>
	void Raven::SceneManager::AddScene(const std::string& name)
	{
		LOGI("[{0}] - Enqueued scene : {1}", __FILE__, name.c_str());
		allScenes.emplace_back(std::make_shared<T>(name));
	}

}
