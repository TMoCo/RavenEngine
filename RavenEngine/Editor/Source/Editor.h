//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <entt/entt.hpp>
#include "Engine.h"
#include "EditorWindow.h"
#include "Scene/Component/Transform.h"
#include "Core/EditorCamera.h"
#include "Core/Ray.h"

namespace Raven
{
	class Camera;

	class Editor : public Engine
	{
	public:
		void Initialize() override;
		void OnImGui() override;
		void OnUpdate(float dt) override;

		void SetSelected(const entt::entity& selectedNode);
		void SetCopiedEntity(const entt::entity& selectedNode,bool cut = false);
		inline auto& GetSelected() const { return selectedNode; }
		inline auto& GetCopiedEntity() const  { return copiedNode; }
		inline auto IsCutCopyEntity() const { return cutCopyEntity; }
		inline auto& GetComponentIconMap() const { return iconMap; }
		inline auto& GetComponentIconMap(){ return iconMap; }

		void OnSceneCreated(Scene* scene) override;

		inline auto GetImGuizmoOperation() const { return imGuizmoOperation; }
		inline auto SetImGuizmoOperation(uint32_t imGuizmoOperation) { this->imGuizmoOperation = imGuizmoOperation; }

		inline auto& GetCamera() { return camera; }
		inline auto& GetEditorCameraTransform()	{return editorCameraTransform;}

		inline auto& GetEditorCameraController() { return editorCameraController; }

		void OnImGuizmo();

	

		Ray SendScreenRay(int32_t x, int32_t y, Camera* camera, int32_t width, int32_t height);
		void SelectObject(const Ray& ray);
	private:
		void DrawMenu();
		void BeginDockSpace();
		void EndDockSpace();

		void LoadCachedScene();
		void CacheScene();

		std::vector<std::unique_ptr<EditorWindow>> editorWindows;
		entt::entity selectedNode = entt::null;
		entt::entity copiedNode = entt::null;
		bool cutCopyEntity = false;
		std::unordered_map<size_t, const char*> iconMap;

		uint32_t imGuizmoOperation = 0;

		bool showGizmos = true;

		std::unique_ptr<Camera> camera;
		Transform editorCameraTransform;
		EditorCameraController editorCameraController;

	};

};
