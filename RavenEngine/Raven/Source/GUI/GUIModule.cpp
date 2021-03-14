#include "GUIModule.h"

namespace Raven {

	GUIModule::GUIModule()
	{

	}

	GUIModule::~GUIModule()
	{
	}

	/** Initialise the module, inluding ImGui
	*	SetWindow must be called before the initialisation will work
	*/
	void GUIModule::Initialize()
	{
		// Get a list of all the fonts loaded by the GUI
		ImGuiIO& io = ImGui::GetIO();
		loadedFonts.push_back(io.Fonts->AddFontDefault());

		// This font loading is a temporary system but works for now
		loadFont("../RavenEngine/Raven/Source/GUI/FONTS_TEMP/DroidSans.ttf", 15);
	}

	/** Responsible for cleaning up the ImGui context and any other memory used by the module
	*	Must be called before destruction of the glfwWindow();
	*/
	void GUIModule::Destroy()
	{
	}

	size_t GUIModule::loadFont(const char* file_name, int pixel_size) {
		ImGuiIO& io = ImGui::GetIO();
		loadedFonts.push_back(io.Fonts->AddFontFromFileTTF(file_name, pixel_size));
		return loadedFonts.size() - 1;
	}

	ImFont* GUIModule::getFont(size_t font_i) {
		// Could handle errors here
		if (loadedFonts.size() <= font_i) return NULL;

		return loadedFonts[font_i];
	}
	
	//Functionality is in ImGui module
	
	/** Frame Begining Function
	*   Should be called at the begining of every frame before any widget specification code
	*/
	void GUIModule::BeginFrame()
	{
		panelCount = 0;
	}
    
	/** Frame Ending Function
	*   Should be called after all widget specification functions are called, before displaying to screen
	*/
	void GUIModule::EndFrame()
	{

	}
    


	void GUIModule::BeginPanel(GUIPanelConfig config)
	{
		int winW, winH;
		glfwGetWindowSize(Engine::GetModule<Raven::Window>()->GetNativeWindow(), &winW, &winH);

		// Style the Window
		// Using push/pop style will allow us to not affect the style in the rest of the programs ImGui usage
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, config.rounding);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, config.padding);
		ImGui::PushStyleColor(ImGuiCol_Border, config.border_col);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, config.bg_col);

		ImGui::SetNextWindowPos(config.position);
		ImGui::SetNextWindowSize(config.size);

		char panelName[33];
		char* panelText = "Panel ";
		sprintf(panelName, "%s %d", panelText, panelCount++);

		// Define the window
		ImGui::Begin(panelName, NULL, DefaultFlags);

	}

	void GUIModule::EndPanel()
	{
		ImGui::End();

		// Revert the panel styles
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(2);
	}

	void GUIModule::Text(GUITextConfig config, const char* fmt, ...)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, config.color);
		ImGui::PushFont(config.font);


		// Horizontal Text Alignment Logic
		if (config.text_align.x != 0) {
			ImGui::Spacing();
			// Calculate the position of the right alligned button
			float b_width = ImGui::CalcTextSize(fmt).x;
			ImGui::SameLine(config.text_align.x * (ImGui::GetWindowWidth() - b_width));
		}


		va_list args;
		va_start(args, fmt);
		// Pass the format string and the args to the ve_list compatible text
		ImGui::TextV(fmt, args);
		va_end(args);

		ImGui::PopFont();
		ImGui::PopStyleColor();
	}

	bool GUIModule::Button(GUIButtonConfig b_config, GUITextConfig t_config, const char* text)
	{
		// Configure the button
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, t_config.text_align);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, b_config.rounding);
		ImGui::PushStyleColor(ImGuiCol_Button, b_config.color);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, b_config.color_hover);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, b_config.color_active);
		ImGui::PushStyleColor(ImGuiCol_Text, t_config.color);

		// Configure the button text
		ImGui::PushFont(t_config.font);

		// Button Alignment Logic
		if (b_config.h_align != 0) {
			ImGui::Spacing();
			// Calculate the position of the right alligned button
			float b_width = ImGui::CalcTextSize(text).x + (2 * ImGui::GetStyle().FramePadding.x);
			if (b_config.size.x > b_width) b_width = b_config.size.x;
			ImGui::SameLine(b_config.h_align * (ImGui::GetWindowWidth() - (b_width + ImGui::GetStyle().WindowPadding.x)));
		}

		bool ret = ImGui::Button(text, b_config.size);

		ImGui::PopFont();
		ImGui::PopStyleColor(4);
		ImGui::PopStyleVar(2);

		return ret;
	}

	ImDrawData* GUIModule::GetDrawData() {
		return ImGui::GetDrawData();
	}
}