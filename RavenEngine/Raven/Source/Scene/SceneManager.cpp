#include "SceneManager.h"

#include "Utilities/StringUtils.h"
#include "Scene/Scene.h"
#include "Engine.h"
#include "Entity/Entity.h"

#include "ResourceManager/ResourceManager.h"

#include <filesystem>




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
		//SaveCurrentScene();
	}

	scenes.clear();
}


void SceneManager::SwitchToScene(const Scene* scene)
{
	bool found = false;
	switchingScenes = true;
	int32_t sceneIndex = GetSceneIndex(scene);

	if (sceneIndex == -1)
	{
		LOGW("Failed Switch To Scene, scene is not added to the manager.");
		return;
	}

	SwitchToScene(sceneIndex);
}



void SceneManager::SwitchToScene(int32_t index)
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
		CreateOrLoadDefaultScene();
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


Scene* SceneManager::GetSceneByName(const std::string& sceneName) 
{
	for (auto& scene : scenes)
	{
		if (scene->GetName() == sceneName) 
			return scene.get();
	}

	return nullptr;
}


Scene* SceneManager::GetScene(int32_t index)
{
	if (index >= 0 && index < scenes.size())
		return scenes[index].get();

	return nullptr;
}


int32_t SceneManager::GetSceneIndex(const Scene* inScene)
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
	scenes.emplace_back(scene);
	return scenes.size() - 1;
}


void SceneManager::RemoveScene(const Scene* scene)
{
	RAVEN_ASSERT(currentScene != scene, "Can't remove current scene");

	int32_t index = GetSceneIndex(scene);

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


void SceneManager::SaveCurrentScene()
{
	RAVEN_ASSERT(currentScene != nullptr, "Can't save invalid scene");

	// The current scene to save.
	Ptr<Scene> scene = scenes[GetSceneIndex(currentScene)];

	// New Unsaved Scene
	if (Engine::GetModule<ResourceManager>()->HasResource(scene))
	{
		Engine::GetModule<ResourceManager>()->SaveResource(scenes[GetSceneIndex(currentScene)]);
	}
	else
	{
		Engine::GetModule<ResourceManager>()->SaveNewResource(scene, "./scenes/" + scene->GetName() + ".raven");
	}
	
}


Ptr<Scene> SceneManager::LoadScene(const std::string& path)
{
	Ptr<Scene> rscScene = Engine::GetModule<ResourceManager>()->GetResource<Scene>(path);
	scenes.emplace_back(rscScene);

	return rscScene;
}


void SceneManager::UnloadScenes()
{
	// Current Scene? Cleanup...
	if (currentScene != nullptr)
	{
		currentScene->OnClean();
		currentScene = nullptr;
		currentSceneIndex = 0;
	}

	// Unload from Resource Manager.
	for (auto scene : scenes)
	{
		Engine::GetModule<ResourceManager>()->UnloadResource(scene);
	}

	// Clear all references to the scenes.
	scenes.clear();
}


void SceneManager::CreateOrLoadDefaultScene()
{
	if (Engine::GetModule<ResourceManager>()->HasResource(DEFAULT_SCENE))
	{
		Ptr<Scene> defaultScene = Engine::GetModule<ResourceManager>()->GetResource<Scene>(DEFAULT_SCENE);
		scenes.emplace_back(defaultScene);
	}
	else
	{
		Ptr<Scene> defaultScene = AddScene<Scene>("Default");
		scenes.emplace_back(defaultScene);
	}
}




} // End of namespace Raven.

