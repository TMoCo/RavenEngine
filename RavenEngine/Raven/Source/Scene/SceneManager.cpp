//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
// Copyright ?2021-2023 Prime Zeng                                          //
//////////////////////////////////////////////////////////////////////////////
#include "SceneManager.h"
#include "Utilities/StringUtils.h"
#include "Scene/Scene.h"
#include "Engine.h"
#include "Entity/Entity.h"
#include "ResourceManager/Resources/Model.h"

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
		}

		if (currentScene == nullptr && allScenes.size() > 0) 
		{
			currentScene = allScenes.back().get();
			currentScene->OnInit();
		}
		else if (currentScene != nullptr) 
		{
			currentScene->OnClean();
			currentScene = allScenes.back().get();
			currentScene->OnInit();
		}

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

	void SceneManager::AddScene(Scene* scene)
	{
		allScenes.emplace_back(std::shared_ptr<Scene>(scene));
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
