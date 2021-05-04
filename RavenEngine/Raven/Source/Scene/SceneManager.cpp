#include "SceneManager.h"

#include "Utilities/StringUtils.h"
#include "Scene/Scene.h"
#include "Engine.h"
#include "Entity/Entity.h"




namespace Raven {


SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}


void SceneManager::Initialize()
{

}


void SceneManager::Destroy()
{
	if (currentScene->GetName() == "Default")
	{
		currentScene->Save(currentScene->path);
	}

	scenes.clear();
}


void SceneManager::AddFileToLoadList(const std::string & filePath)
{
	sceneFilePathsToLoad.emplace_back(filePath);
}


void SceneManager::SwitchToScene(const std::string& name)
{
	bool found = false;
	switchingScenes = true;
	uint32_t idx = 0;

	for (uint32_t i = 0; !found && i < scenes.size(); ++i)
	{
		if (!scenes[i])
			continue;

		if (scenes[i]->GetName() == name)
		{
			found = true;
			idx = i;
			break;
		}
	}

	if (found)
	{
		SwitchToScene(idx);
	}
	else
	{
		LOGW("[{0} : {1}] - Unknown Scene : {2}",__FILE__,__LINE__, name.c_str());
	}
}



void SceneManager::SwitchToScene(uint32_t index)
{
	queuedSceneIndex = index;
	switchingScenes = true;
}


void SceneManager::Apply()
{
	// Nothing to apply?
	if (!switchingScenes && currentScene)
		return;

	// Empty? Create or Default scene...
	if (scenes.empty())
	{
		auto defaultScene = AddScene<Scene>("Default");
		defaultScene->path = StringUtils::GetCurrentWorkingDirectory() + "/scenes/Default.raven";

		// Exist?
		struct stat fileInfo;
		if ((!stat(defaultScene->path.c_str(), &fileInfo)))
		{
			defaultScene->isNeedLoading = true;
		}
		else
		{
			defaultScene->isNeedLoading = false;
		}

		queuedSceneIndex = 0;
	}

	// Current Scene? Cleanup...
	if (currentScene != nullptr)
	{
		currentScene->OnClean();
		currentScene = nullptr;
	}

	currentSceneIndex = queuedSceneIndex;
	currentScene = scenes[queuedSceneIndex].get();

	// Does the scene need to be loaded from file.
	if (currentScene->isNeedLoading)
	{
		currentScene->Load( currentScene->path );
	}


	if(Engine::Get().GetEditorState() == EditorState::Play)
		currentScene->OnInit();


	Engine::Get().OnSceneCreated(currentScene);

	switchingScenes = false;
}


std::vector<std::string> SceneManager::GetSceneNames()
{
	std::vector<std::string> names;
	for (auto& scene : scenes)
	{
		names.emplace_back(scene->GetName());
	}
	return names;
}


void SceneManager::AddSceneFromFile(const std::string& filePath)
{
	sceneFilePaths.emplace_back(filePath);
	auto name = StringUtils::RemoveExtension(StringUtils::GetFileName(filePath));
	auto newScene = AddScene<Scene>(name);
	newScene->isNeedLoading = true;
}


Scene* SceneManager::GetSceneByName(const std::string& sceneName) 
{
	for (auto& scene : scenes)
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


Scene* SceneManager::GetScene(int32_t index)
{
	if (index >= 0 && index < scenes.size())
		return scenes[index].get();

	return nullptr;
}


int32_t SceneManager::GetSceneIndex(Scene* inScene)
{
	if (!inScene)
		return -1;

	for (int32_t i = 0; i < (int32_t)scenes.size(); ++i)
	{
		if (!scenes[i])
			continue;

		if (scenes[i].get() == inScene)
			return i;
	}

	return -1;
}


int32_t SceneManager::GetSceneIndex(const std::string& sceneName)
{
	for (int32_t i = 0; i < (int32_t)scenes.size(); ++i)
	{
		if (!scenes[i])
			continue;

		if (scenes[i]->GetName() == sceneName)
			return i;
	}

	return -1;
}


int32_t SceneManager::AddScene(Scene* scene)
{
	RAVEN_ASSERT(GetSceneIndex(scene->GetName()) == -1, "Scene name already exist.");
	scenes.emplace_back(scene);
}


void SceneManager::RemoveScene(const std::string& name)
{
	RAVEN_ASSERT(currentScene->GetName() != name, "Can't remove current scene");

	int32_t index = GetSceneIndex(name);

	if (!GetScene(index))
		return;


	// Is Last Scene?
	if (index == scenes.size()- 1)
	{
		scenes.pop_back();
	}
	else
	{
		// Just reset the pointer to keep the scene indicies valid
		scenes[index].reset();
	}

}




} // End of namespace Raven.

