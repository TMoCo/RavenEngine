#include "Material.h"
#include "ResourceManager/Resources/Texture2D.h"
#include "ResourceManager/Resources/MaterialShader.h"
#include "Render/RenderResource/Shader/RenderRscMaterial.h"




namespace Raven
{

Material::Material(Ptr<MaterialShader> inShader)
	: IResource(EResourceType::RT_Material, true)
	, shader(inShader)
	, renderRsc(nullptr)
	, dirtyFlag(EMaterialDirtyFlag::None)
{

}


Material::~Material()
{
	delete renderRsc;
}


int32_t Material::SetTexture(const std::string& name, Texture2D* texture)
{
	int32_t idx = GetTextureIndex(name);

	// New?
	if (idx == -1)
	{
		idx = static_cast<int32_t>(textures.size());
		textures.push_back(std::make_pair(name, texture));
		texturesMap.insert(std::make_pair(name, idx));
		DirtyRemap();
	}
	else
	{
		textures[idx].second = texture;
		DirtyUpdate();
	}


	return idx;
}


int32_t Material::SetScalar(const std::string& name, const float& scalar)
{
	int32_t idx = GetScalarIndex(name);

	// New?
	if (idx == -1)
	{
		idx = static_cast<int32_t>(scalars.size());
		scalars.push_back(std::make_pair(name, scalar));
		scalarsMap.insert(std::make_pair(name, idx));
		DirtyRemap();
	}
	else
	{
		scalars[idx].second = scalar;
		DirtyUpdate();
	}


	return idx;
}


int32_t Material::SetColor(const std::string& name, const glm::vec4& color)
{
	int32_t idx = GetColorIndex(name);

	// New?
	if (idx == -1)
	{
		idx = static_cast<int32_t>(colors.size());
		colors.push_back(std::make_pair(name, color));
		colorsMap.insert(std::make_pair(name, idx));
		DirtyRemap();
	}
	else
	{
		colors[idx].second = color;
		DirtyUpdate();
	}


	return idx;
}



void Material::SetTexture(int32_t idx, Texture2D* texture)
{
	DirtyUpdate();
	textures[idx].second = texture;
}


void Material::SetScalar(int32_t idx, const float& scalar)
{
	DirtyUpdate();
	scalars[idx].second = scalar;
}


void Material::SetColor(int32_t idx, const glm::vec4& color)
{
	DirtyUpdate();
	colors[idx].second = color;
}


int32_t Material::GetTextureIndex(const std::string& name) const
{
	// Search...
	auto iter =  texturesMap.find(name);

	// Not Found?
	if (iter == texturesMap.end())
		return -1;

	return iter->second;
}


int32_t Material::GetScalarIndex(const std::string& name) const
{
	// Search...
	auto iter = scalarsMap.find(name);

	// Not Found?
	if (iter == scalarsMap.end())
		return -1;

	return iter->second;
}


int32_t Material::GetColorIndex(const std::string& name) const
{
	// Search...
	auto iter = colorsMap.find(name);

	// Not Found?
	if (iter == colorsMap.end())
		return -1;

	return iter->second;
}


bool Material::GetTexture(const std::string& name, Texture2D*& outTexture) const
{
	int32_t idx = GetTextureIndex(name);
	if (idx == -1)
		return false;

	outTexture = textures[idx].second;
	return true;
}


bool Material::GetScalar(const std::string& name, float& outScalar) const
{
	int32_t idx = GetScalarIndex(name);
	if (idx == -1)
		return false;

	outScalar = scalars[idx].second;
	return true;
}


bool Material::GetColor(const std::string& name, glm::vec4& outColor) const
{
	int32_t idx = GetColorIndex(name);
	if (idx == -1)
		return false;

	outColor = colors[idx].second;
	return true;
}


void Material::DirtyUpdate()
{
	if (dirtyFlag == EMaterialDirtyFlag::None)
		dirtyFlag = EMaterialDirtyFlag::Update;
}


void Material::DirtyRemap()
{
	dirtyFlag = EMaterialDirtyFlag::Remap;
}


void Material::LoadOnGpu()
{
	RAVEN_ASSERT(!onGPU, "Material already loaded.");
	onGPU = true;

	// Render Material
	renderRsc = new RenderRscMaterial(shader->GetRenderRsc());
	renderRsc->LoadInputBlock(shader->GetBlockInput().name);
	renderRsc->SetUniformBuffer(shader->GetUnifromBuffer());

	Update();
}


void Material::Update()
{
	RAVEN_ASSERT(onGPU, "Material not loaded on gpu.");

	// Remapping...
	if (dirtyFlag == EMaterialDirtyFlag::Remap)
	{
		renderRsc->ClearMapping();

		for (auto& param : textures)
			renderRsc->MapParamter(param.first, &param.second);

		for (auto& param : scalars)
			renderRsc->MapParamter(param.first, &param.second);

		for (auto& param : colors)
			renderRsc->MapParamter(param.first, &param.second);
	}

	// Update Buffer.
	renderRsc->FillBuffer();

	// Clear Flag
	dirtyFlag = EMaterialDirtyFlag::None;
}



} // End of namespace Raven.


