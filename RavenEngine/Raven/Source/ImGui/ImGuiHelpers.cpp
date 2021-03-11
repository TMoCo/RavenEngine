#include "ImGuiHelpers.h"


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
};



