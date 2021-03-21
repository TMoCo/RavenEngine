

#include "Light.h"
#include <imgui.h>
#include "ImGui/ImGuiHelpers.h"

namespace Raven
{

	std::string LightTypeToString(LightType type)
	{
		switch (type)
		{
		case LightType::DirectionalLight:
			return "Directional Light";
		case LightType::SpotLight:
			return "Spot Light";
		case LightType::PointLight:
			return "Point Light";
		default:
			return "ERROR";
		}
	}

	int32_t StringToLightType(const std::string& type)
	{
		if (type == "Directional")
			return int32_t(LightType::DirectionalLight);

		if (type == "Point")
			return int32_t(LightType::PointLight);

		if (type == "Spot")
			return int32_t(LightType::SpotLight);

		return 0.0f;
	}


	Light::Light(const glm::vec3& direction, const glm::vec4& color, float intensity, const LightType& type, const glm::vec3& position, float radius, float angle)
		: direction(direction)
		, color(color)
		, position(position)
		, intensity(intensity)
		, radius(radius)
		, type(int32_t(type))
		, angle(angle)
	{
	}
	
	void Light::OnImGui()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();

		if (type != 0)
			ImGuiHelper::Property("Position", position);

		if (type != 2)
			ImGuiHelper::Property("Direction", direction);

		if (type != 0)
			ImGuiHelper::Property("Radius", radius, 0.0f, 100.0f);
		ImGuiHelper::Property("Color", color, true, ImGuiHelper::PropertyFlag::ColorProperty);
		ImGuiHelper::Property("Intensity", intensity, 0.0f, 100.0f);

		if (type == 1)
			ImGuiHelper::Property("Angle", angle, -1.0f, 1.0f);

		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted("Light Type");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
	

		if (ImGui::BeginMenu(LightTypeToString(LightType(int32_t(type))).c_str()))
		{
			if (ImGui::MenuItem("Directional Light", "", static_cast<int>(type) == 0, true))
			{
				type = int32_t(LightType::DirectionalLight);
			}
			if (ImGui::MenuItem("Spot Light", "", static_cast<int>(type) == 1, true))
			{
				type = int32_t(LightType::SpotLight);
			}
			if (ImGui::MenuItem("Point Light", "", static_cast<int>(type) == 2, true))
			{
				type = int32_t(LightType::PointLight);
			}
			ImGui::EndMenu();
		}


		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
};