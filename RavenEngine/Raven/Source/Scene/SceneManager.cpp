//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
// Copyright ?2021-2023 Prime Zeng                                          //
//////////////////////////////////////////////////////////////////////////////
#include "SceneManager.h"
#include "Utilities/StringUtils.h"
#include "Scene/Scene.h"
#include "Engine.h"
#include "Entity/Entity.h"
#include "Scene/Component/Model.h"

namespace Raven 
{
	void SceneManager::AddFileToLoadList(const std::string & filePath)
	{
		sceneFilePathsToLoad.emplace_back(filePath);
	}

	void SceneManager::SwitchScene()
	{
		SwitchScene((sceneIdx + 1) % allScenes.size());
	}

	void SceneManager::SwitchScene(const std::string& name)
	{
		bool found = false;
		switchingScenes = true;
		uint32_t idx = 0;

		for (uint32_t i = 0; !found && i < allScenes.size(); ++i)
		{
			if (allScenes[i]->GetName() == name)
			{
				found = true;
				idx = i;
				break;
			}
		}

		if (found)
		{
			SwitchScene(idx);
		}
		else
		{
			LOGW("[{0} : {1}] - Unknown Scene : {2}",__FILE__,__LINE__, name.c_str());
		}
	}

	void SceneManager::SwitchScene(uint32_t index)
	{
		queuedSceneIndex = index;
		switchingScenes = true;
	}

	void SceneManager::Apply()
	{
		if (switchingScenes == false)
		{
			if (currentScene != nullptr)
				return;
			if (allScenes.empty()) { 
				AddScene(new Scene("default"));
			}
			queuedSceneIndex = 0;
		}

		if (currentScene != nullptr) //clear before
		{
			currentScene->OnClean();
			currentScene = allScenes.back().get();
		}

		currentScene = allScenes[queuedSceneIndex].get();

		if (!currentScene->dynamic)
			currentScene->Load("./scenes/");

		if(Engine::Get().GetEditorState() == EditorState::Play)
			currentScene->OnInit();

		Engine::Get().OnSceneCreated(currentScene);

		switchingScenes = false;
	}

	std::vector<std::string> SceneManager::GetSceneNames()
	{
		std::vector<std::string> names;
		for (auto& scene : allScenes)
		{
			names.emplace_back(scene->GetName());
		}
		return names;
	}

	void SceneManager::AddSceneFromFile(const std::string& filePath)
	{
		sceneFilePaths.emplace_back(filePath);
		auto name = StringUtils::RemoveExtension(StringUtils::GetFileName(filePath));
		auto scene = new Scene(name);
		AddScene(scene);
	}

	uint32_t SceneManager::AddScene(Scene* scene)
	{
		allScenes.emplace_back(std::shared_ptr<Scene>(scene));
		return allScenes.size() - 1;
	}

	// Function to return a scene given its name - Ben
	Scene* SceneManager::GetSceneByName(const std::string& sceneName) 
	{
		for (auto& scene : allScenes)
		{
			if (scene->GetName() == sceneName) 
				return scene.get();
		}
		return nullptr;
	}


	void SceneManager::LoadCurrentList()
	{
		for (auto& filePath : sceneFilePathsToLoad)
		{
			AddSceneFromFile(filePath);
		}
		sceneFilePathsToLoad.clear();
	}

	void SceneManager::Initialize()
	{

	}

	void SceneManager::Destroy()
	{

	}
};
