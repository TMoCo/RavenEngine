
#pragma once
#include <entt/entt.hpp>
#include "Engine.h"
#include "EditorWindow.h"

namespace Raven
{

	class Editor : public Engine
	{
	public:
		void Initialize() override;
		void OnImGui() override;
		void SetSelected(const entt::entity& selectedNode);
		void SetCopiedEntity(const entt::entity& selectedNode,bool cut = false);
		inline auto& GetSelected() const { return selectedNode; }
		inline auto& GetCopiedEntity() const  { return copiedNode; }
		inline auto IsCutCopyEntity() const { return cutCopyEntity; }
		inline auto& GetComponentIconMap() const { return iconMap; }

	private:
		void DrawMenu();
		void BeginDockSpace();
		void EndDockSpace();
		std::vector<std::unique_ptr<EditorWindow>> editorWindows;
		entt::entity selectedNode = entt::null;
		entt::entity copiedNode = entt::null;
		bool cutCopyEntity = false;
		std::unordered_map<size_t, const char*> iconMap;
	};

};
