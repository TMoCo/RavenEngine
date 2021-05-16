#pragma  once


#include "EditorWindow.h"
#include "Utilities/Core.h"
#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/Shader/RenderShaderInput.h"



namespace Raven
{

	// ResourceWindow:
	//    - View & Edit raven resources. 
	//
	class ResourceWindow : public EditorWindow
	{
	public:
		// Construct.
		ResourceWindow();

		// Destruct.
		virtual ~ResourceWindow();

		// Draw UI.
		void OnImGui();

		// Set path of a resource to be viewed and edited.
		void SetResource(const std::string& rscPath);

		// Set a resource to be viewed and edited.
		void SetResource(Ptr<IResource> rsc);

	private:
		// UI - Texture2D.
		void OnImGuiTexture2D();

		// UI - Shader.
		void OnImGuiShader();

		// UI - Material.
		void OnImGuiMaterial();

		// UI - Mesh
		void OnImGuiMesh();

		// UI - Skinned
		void OnImGuiSkinned();

		// ImGui Util function for drag and drop.
		std::string ImGuiDragDropAssetsTarget();

		// Draw shader input UI.
		bool ImGuiShaderInputUI(int32_t index, const RSInputDescription& input, RSInputDescription& outEdit, bool& outRm);

		// ReCreate Input Block to match the new edit.
		RSInputBlockDescription ReCreateBlock(const RSInputBlockDescription& block, int32_t editIndex, const RSInputDescription& edit, bool rm, bool isNew);

	private:
		// The selected resource.
		Ptr<IResource> resource;

		// The selected resource type.
		EResourceType type;

	};

}