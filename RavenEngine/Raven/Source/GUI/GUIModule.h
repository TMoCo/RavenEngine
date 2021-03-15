#pragma once


#include "IModule.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger/Console.h"


#include <condition_variable>
#include <GLFW/glfw3.h>
#include "Engine.h"
#include "Window/Window.h"

#include <vector>

namespace Raven {

	class GUIModule : public IModule
	{
	public:
		// Widget Configuration Structs
		struct GUIPanelConfig {
			// Configuration variables
			ImVec4 bg_col = ImVec4(0., 0., 0., 0.);
			ImVec4 border_col = ImVec4(0., 0., 0., 0.);
			ImVec2 padding = ImVec2(0., 0.); // Internal padding of panel given in pixels
			ImVec2 position = ImVec2(0., 0.); // Position in pizels of top left
			ImVec2 size = ImVec2(0., 0.); // (0,0) will make panel auto size
			float rounding = 0;

			// Constructor to get ImGui's current styles
			GUIPanelConfig() {
				bg_col = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
				border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
				padding = ImGui::GetStyle().WindowPadding;
				rounding = ImGui::GetStyle().WindowRounding;
			}
		};
		struct GUITextConfig {
			ImVec4 color = ImVec4(0., 0., 0., 0.);
			ImVec2 text_align = ImVec2(0., 0.); // Text align given as percentages in x, and y direction
			ImFont* font = NULL;

			GUITextConfig() {
				color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
				font = ImGui::GetFont();
			}
		};
		struct GUIButtonConfig {
			ImVec4 color = ImVec4(0., 0., 0., 0.);
			ImVec4 color_hover = ImVec4(0., 0., 0., 0.);
			ImVec4 color_active = ImVec4(0., 0., 0., 0.);
			ImVec2 size = ImVec2(0., 0.); // If a dimention is = 0 then that axis will auto size to fit the text
			float h_align = 0; // Horizontal alignment, percentage based
			float rounding = 0; // Horizontal alignment, percentage based

			GUIButtonConfig() {
				color = ImGui::GetStyleColorVec4(ImGuiCol_Button);
				color_hover = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
				color_active = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
				rounding = ImGui::GetStyle().FrameRounding;
			}
		};

	public:
		/** Constuctor */
		GUIModule();

		/** Destructor */
		~GUIModule();

		/** Return the type of the module. */
		static EModuleType GetModuleType() { return MT_GUI; }
	
	private:
		/** Module Initialize. */
		virtual void Initialize() override;

		/** Module Destroy. */
		virtual void Destroy() override;

	public:

		void BeginFrame();

		void EndFrame();

		void BeginPanel(GUIPanelConfig config);

		void EndPanel();

		void Text(GUITextConfig config, const char* fmt, ...);

		bool Button(GUIButtonConfig b_config, GUITextConfig t_config, const char* text);

		ImDrawData* GetDrawData();

		ImFont* getFont(size_t font_i);

		size_t loadFont(const char* file_name, int pixel_size);

	private:

		GLFWwindow* window = NULL;

		/** A default window/panel configuration
		*	This panel will have no title, scrollbar
		*	It will be unmovable and non resizable*/
		const ImGuiWindowFlags DefaultFlags = 0
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoNav
			| ImGuiWindowFlags_AlwaysAutoResize;

		int panelCount = 0;

		std::vector<ImFont*> loadedFonts;
	};

}