#include "FbxWindow.h"

#include "ResourceManager/FileSystem.h"
#include "Utilities/StringUtils.h"
#include "Logger/Console.h"

namespace Raven 
{
	FbxWindow::FbxWindow()
	{
		title = "FbxWindow";
	}


	void FbxWindow::OpenFile(const std::string& path)
	{
		if (scene != nullptr) 
		{
			scene->destroy();
			scene = nullptr;
		}
		std::string err;
		std::string name = StringUtils::GetFileName(path);
		std::string ext = StringUtils::GetExtension(name);
		int64_t size = 0;
		auto data = FileSystem::ReadFile(path, size);

		if (data == nullptr)
		{
			LOGW("Failed to load fbx file"); return;
		}
		const bool ignoreGeometry = false;
		const uint64_t flags = ignoreGeometry ? (uint64_t)ofbx::LoadFlags::IGNORE_GEOMETRY : (uint64_t)ofbx::LoadFlags::TRIANGULATE;
		scene = ofbx::load(data.get(), uint32_t(size), flags);
	}
	void FbxWindow::OnImGui()
	{
		if (ImGui::Begin(title.c_str(),&active))
		{
			if (scene != nullptr) 
			{
				const ofbx::Object* root = scene->getRoot();
				if (root) ShowObjectGUI(*root);
				auto count = scene->getAnimationStackCount();
				for (auto i = 0; i < count; ++i)
				{
					auto stack = scene->getAnimationStack(i);
					ShowObjectGUI(*stack);
				}
			}
		}
		ImGui::End();
	}

	void FbxWindow::ShowObjectGUI(const ofbx::Object& object)
	{
		const char* label;
		switch (object.getType())
		{
		case ofbx::Object::Type::GEOMETRY: label = "geometry"; break;
		case ofbx::Object::Type::MESH: label = "mesh"; break;
		case ofbx::Object::Type::MATERIAL: label = "material"; break;
		case ofbx::Object::Type::ROOT: label = "root"; break;
		case ofbx::Object::Type::TEXTURE: label = "texture"; break;
		case ofbx::Object::Type::NULL_NODE: label = "null"; break;
		case ofbx::Object::Type::LIMB_NODE: label = "limb node"; break;
		case ofbx::Object::Type::NODE_ATTRIBUTE: label = "node attribute"; break;
		case ofbx::Object::Type::CLUSTER: label = "cluster"; break;
		case ofbx::Object::Type::SKIN: label = "skin"; break;
		case ofbx::Object::Type::ANIMATION_STACK: label = "animation stack"; break;
		case ofbx::Object::Type::ANIMATION_LAYER: label = "animation layer"; break;
		case ofbx::Object::Type::ANIMATION_CURVE: label = "animation curve"; break;
		case ofbx::Object::Type::ANIMATION_CURVE_NODE: label = "animation curve node"; break;
		default: assert(false); break;
		}

		ImGuiTreeNodeFlags flags = currentSelect == &object ? ImGuiTreeNodeFlags_Selected : 0;
		char tmp[128];
		sprintf_s(tmp, "%lld %s (%s)", object.id, object.name, label);
		if (ImGui::TreeNodeEx(tmp, flags))
		{
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) currentSelect = &object;
			int i = 0;
			while (ofbx::Object* child = object.resolveObjectLink(i))
			{
				ShowObjectGUI(*child);
				++i;
			}
			if (object.getType() == ofbx::Object::Type::ANIMATION_CURVE) {
				ShowObjectGUI(object);
			}

			ImGui::TreePop();
		}
		else
		{
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) currentSelect = &object;
		}
	}
};

