#pragma once




#include "Utilities/Core.h"






namespace Raven
{
	class RenderSphere;
	class RenderScreen;
	class RenderPass;
	class RenderRscShader;
	class RenderRscTexture;



	// RenderTexFilter:
	//		- provide functionalities for generate/pre-filter textures.
	//    - Filter IBL, 
	//
	//		- Only one instance created/managed by the RenderModule.
	//
	class RenderTexFilter
	{
		// Friend...
		friend class RenderModule;

		// Private Construct, @see RenderModule.
		RenderTexFilter(Ptr<RenderScreen> screen, Ptr<RenderSphere> sphere);

	public:
		// Destruct
		~RenderTexFilter();

		// Initializie the filters.
		void Initialize();

		// Destroyo the filters
		void Destroy();

		// Generate CubeMap texture from Equirectangular/Spherical Texturs.
		void GenCubeMap(RenderRscTexture* inTexture, RenderRscTexture* outCubeMap, bool isHDR);

		// Filter an ennvionment image for IBL irradiance.
		void FilterIrradianceIBL(RenderRscTexture* inEnvTexture, RenderRscTexture* outIrradiance);

		// Filter an ennvionment image for IBL specular.
		void FilterSpecularIBL(RenderRscTexture* inEnvTexture, RenderRscTexture* outSpecular);

		// Generate BRDF look up table.
		void GenBRDFLUT(RenderRscTexture* outBRDF);

	private:
		// Create the shader used for converting spherical textures into cubemapss
		void CreateCubeMapGenShader();

		// Create the shader used for converting environment maps into filterd irradiance.
		void CreateIrradainceFilterShader();

		// Create the shader used for converting environment maps into filterd specular.
		void CreateSepcularFilterShader();

		//
		void CreateBRDFLUTShader();

	private:
		// Screen triangle used to render the entire screen.
		Ptr<RenderScreen> rscreen;

		// Sphere.
		Ptr<RenderSphere> rsphere;

		// Render Pass used by multiple filter opeations a single texture.
		Ptr<RenderPass> filterPass;

		// Shader for generating cubemap from spherical/Equirectangular textures.
		Ptr<RenderRscShader> cubeMapGenShader;

		// Shader for computing pre-filterd irradiance map.
		Ptr<RenderRscShader> filterIrradianceShader;

		// Shader for computing pre-filterd specular map.
		Ptr<RenderRscShader> filterSpecularShader[2];

		// Shader for generating BRDF Look up table.
		Ptr<RenderRscShader> brdfLUTShader;
	};

}
