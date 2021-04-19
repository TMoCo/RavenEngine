//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma  once
#include <string>
#include <imgui.h>
#include <memory>
#include <vector>
#include <imgui_node_editor.h>
#include "EditorWindow.h"
#include "Animation/AnimationController.h"



namespace Raven
{
	namespace ed = ax::NodeEditor;




	class NodeWindow : public EditorWindow
	{
	public:
		NodeWindow();
		~NodeWindow();
		virtual void OnImGui()  override;
		void OpenFile(const std::string& name);
		void OpenController(const std::shared_ptr<AnimationController>& controller);
		virtual void SaveWorkspace() override;
	private:

		void LoadMeta();

		void LoadAnimation(const std::string& file);
		void HandleQuery();
		void HandleDelete();
		void DrawNode();
		void AddNode(const std::string& name);

		void LeftPanel();
		void RightPanel();

		bool CreateLink(int32_t from, int32_t to);

	
		struct LinkInfo
		{
			ed::LinkId id;
			ed::PinId  inputId;
			ed::PinId  outputId;
		};
		
		struct Node 
		{
			int32_t id;
			ed::PinId inputId;
			ed::PinId outputId;
			std::string nodeName;
			std::string filePath;
		};

		Node* FindNode(int32_t id);

		ed::EditorContext* context = nullptr;
		bool firstFrame = true;
		std::vector<LinkInfo> links;
		std::vector<Node> nodes;
		int32_t nextId = 100;

		std::shared_ptr<AnimationController> controller;
		ImGuiTextFilter filter;


		std::string selectParam;
		bool doubleClick = true;
		std::string settingFile;
	};
};