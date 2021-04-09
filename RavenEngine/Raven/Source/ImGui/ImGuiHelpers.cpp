//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "ImGuiHelpers.h"
#include <glm/gtc/type_ptr.hpp>

namespace ImGuiHelper
{
	void Tooltip(const char* str) 
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::TextUnformatted(str);
			ImGui::EndTooltip();
		}
		ImGui::PopStyleVar();
	}

	bool Property(const std::string& name, bool& value)
	{
		bool updated = false;
		ImGui::TextUnformatted(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if (ImGui::Checkbox(id.c_str(), &value))
			updated = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		return updated;
	}

	bool Property(const std::string& name, float& value, float min, float max, PropertyFlag flags)
	{
		bool updated = false;
		ImGui::TextUnformatted(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if (ImGui::SliderFloat(id.c_str(), &value, min, max))
			updated = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();


		return updated;
	}

	bool Property(const std::string& name, glm::vec2& value, PropertyFlag flags)
	{
		return ImGuiHelper::Property(name, value, -1.0f, 1.0f, flags);
	}

	bool Property(const std::string& name, glm::vec2& value, float min, float max, PropertyFlag flags)
	{
		bool updated = false;

		ImGui::TextUnformatted(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if (ImGui::SliderFloat2(id.c_str(), glm::value_ptr(value), min, max))
			updated = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return updated;
	}

	bool Property(const std::string& name, glm::vec3& value, PropertyFlag flags)
	{
		return ImGuiHelper::Property(name, value, -1.0f, 1.0f, flags);
	}

	bool Property(const std::string& name, glm::vec3& value, float min, float max, PropertyFlag flags)
	{
		bool updated = false;

		ImGui::TextUnformatted(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if ((int)flags & (int)PropertyFlag::ColorProperty)
		{
			if (ImGui::ColorEdit3(id.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs))
				updated = true;
		}
		else
		{
			if (ImGui::SliderFloat3(id.c_str(), glm::value_ptr(value), min, max))
				updated = true;
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return updated;
	}

	bool Property(const std::string& name, glm::vec4& value, bool exposeW, PropertyFlag flags)
	{
		return Property(name, value, -1.0f, 1.0f, exposeW, flags);
	}

	bool Property(const std::string& name, glm::vec4& value, float min /*= -1.0f*/, float max /*= 1.0f*/, bool exposeW /*= false*/, PropertyFlag flags /*= PropertyFlag::None*/)
	{
		bool updated = false;

		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if ((int)flags & (int)PropertyFlag::ColorProperty)
		{
			if (ImGui::ColorEdit4(id.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs))
				updated = true;
		}
		else if ((exposeW ? ImGui::SliderFloat4(id.c_str(), glm::value_ptr(value), min, max) : ImGui::SliderFloat3(id.c_str(), glm::value_ptr(value), min, max)))
			updated = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return updated;
	}

	bool Property(const std::string& name, glm::vec3& value, float min /*= -1.0f*/, float max /*= 1.0f*/, bool exposeW /*= false*/, PropertyFlag flags /*= PropertyFlag::None*/)
	{
		bool updated = false;

		ImGui::TextUnformatted(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if ((int)flags & (int)PropertyFlag::ColorProperty)
		{
			if (ImGui::ColorEdit3(id.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs))
				updated = true;
		}
		else
		{
			if (ImGui::SliderFloat3(id.c_str(), glm::value_ptr(value), min, max))
				updated = true;
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return updated;
	}

	bool Property(const std::string& name, std::string& value)
	{
		bool updated = false;
		ImGui::TextUnformatted(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		static char obj[256] = {};
		strcpy(obj, value.c_str());
		if (ImGui::InputText(id.c_str(), obj, 256)) 
		{
			updated = true;
			value = obj;
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return updated;
	}

};



