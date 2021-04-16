#pragma once
#include <string>
#include "GUI/GUIModule.h"
#include "ResourceManager/ResourceManager.h"

namespace Raven
{
	// Forward decleration
	class GUILayout;

	class GUIWidget {

	public:
		// Attributes defined at creation
		std::string widgetName = "";
		GUIModule::GUIWidgetConfig config;
		std::string text;

		// Attributes initialised post creation
		bool visible = true;
		std::function<void(GUILayout* layout)> callback;
		Ptr<Texture2D> img = NULL;
		size_t fontID = 0; // Should be changed to be a pointer to resource manager font

		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("Widget Name", widgetName),
				cereal::make_nvp("Widget Config", config),
				cereal::make_nvp("Text", text));
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("Widget Name", widgetName),
				cereal::make_nvp("Widget Config", config),
				cereal::make_nvp("Text", text));
		}

		void ToggleVisible()
		{
			if (config.type != GUIModule::WT_Panel)
			{
				LOGE("Cannot make non-panel widget: \"{}\" invisible", widgetName);
				return;
			}
			visible = !visible;
		}

		void SetCallback(const std::function<void(GUILayout* layout)>& bCallback)
		{
			callback = bCallback;
		}

		// Load all of the resources needed for the widget through the resource manager
		void LoadResources()
		{
			// If there is an image to load
			if (config.image_path != "")
			{
				Engine::GetModule<ResourceManager>()->LoadResource<Texture2D>(config.image_path);
				img = Engine::GetModule<ResourceManager>()->GetResource<Texture2D>(config.image_path);
			}

			if (config.font_path != "")
			{
				// Font loading will temporarily be done here by ImGui but should be moved to the resource manager
				fontID = Engine::GetModule<GUIModule>()->loadFont(config.font_path.c_str(), config.font_size);
			}
		}
	};


}