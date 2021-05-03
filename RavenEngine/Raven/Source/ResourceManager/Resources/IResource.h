//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Utilities/Core.h"


#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/polymorphic.hpp"




#define INVALID_RSC_INDEX static_cast<uint32_t>(-1)




namespace Raven
{
	// Types of all the supported resource by the engine.
	enum EResourceType
	{
		// Invalid resource.
		RT_None = -1,

		// Resources - The Value of each enum resource is important and should not change, please add 
		//             new entries but keep the original reserved as it will affect the archiving system.
		RT_Texture2D          = 0,
		RT_TextureCube        = 1,
		RT_DynamicTexture     = 2,
		RT_MaterialShader     = 3,
		RT_Material           = 4,
		RT_Mesh               = 5,
		RT_SkinnedMesh        = 6,
		RT_Terrain            = 7,
		RT_Scene              = 8,
		RT_GuiLayout          = 9,
		RT_Skeleton           = 10,
		RT_AnimationClip      = 11
	};


	// Convert resource type to its name as string.
	inline std::string ToString(EResourceType type)
	{
		switch (type)
		{
		// Textures.
		case RT_Texture2D: return "Texture2D";
		case RT_TextureCube: return "TextureCube";
		case RT_DynamicTexture: return "DynamicTexture";

		// Materials.
		case RT_MaterialShader: "MaterialShader";
		case RT_Material: return "Material";

		// Meshes.
		case RT_Mesh: return "Mesh";
		case RT_SkinnedMesh: return "SkinnedMesh";
		case RT_Terrain: return "Terrain";
		case RT_Skeleton: return "Skeleton";

		// Scene.
		case RT_Scene: return "scene";

		// GUI.
		case RT_GuiLayout: return "GuiLayout";

		// Animation.
		case RT_AnimationClip: return "AnimationClip";

		}

		RAVEN_ASSERT(0, "Invalid resource Type.");
	}




	// IResource:
	//    - The base resource class, pointers of which are stored in the resource register
	//
	//    - The IResource class serves as the base class for all Resources that can go in the resource  
	//			register in the ResourceManager module.Each derived resource is identifiable by an enum value.
	//
	class IResource
	{
		friend class ResourceManager;
		friend class ResourceRef;
		friend class ILoader;
		friend class ResourcesRegistry;

	public:
		// Default Construct.
		IResource() 
			: type(EResourceType::RT_None)
			, hasRenderResources(false)
			, isOnGPU(false) 
			, load_version(0)
			, resourceIndex(INVALID_RSC_INDEX)
		{

		}

		// Destruct.
		virtual ~IResource()
		{

		}

		// Return the resource type.
		inline EResourceType GetType() const noexcept { return type; }

		// Return true if the resource contain render data that needs to be loaded on GPU.
		inline bool HasRenderResources() const noexcept { return hasRenderResources; }

		// Return true if the is loaded on GPU.
		inline bool IsOnGPU() const noexcept { return isOnGPU; }

		// Load the render resource. Note: only for resources with render data.
		virtual void LoadRenderResource() { RAVEN_ASSERT(0, "Invalid Operation."); }

		// Update the render resource. Note: only for resources with render data.
		virtual void UpdateRenderResource() { RAVEN_ASSERT(0, "Invalid Operation."); }

		// Set the resrouce name.
		inline void SetName(const std::string& newName) { name = newName; }

		// Return the resource name.
		inline const std::string& GetName() const { return name; }

		// Serialization Save.
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(name);
		}

		// Serialization Load.
		template<typename Archive>
		void load(Archive& archive)
		{
			archive(name);
		}


	protected:
		// The resource type.
		EResourceType type;

		// If the resource contain/need render resources.
		bool hasRenderResources;

		// Is the resource loaded on GPU.
		bool isOnGPU;

		// The version of the resrouce when loaded.
		uint32_t load_version;

	private:
		// The name of the resource.
		std::string name;

		// The path to the resource on disk.
		std::string path;

		// Index of the resources in the resource registry.
		uint32_t resourceIndex;
	};




	// ResourceRef:
	//    - Used to reference a resource for archiving operations.
	//
	class ResourceRef
	{
		// Friend...
		friend class ResourceManager;

		// No Copy Assignment
		ResourceRef& operator=(const ResourceRef&) = delete;

	private:
		// The Resource relative path.
		std::string path;

		// The type of the Resource.
		EResourceType type;

		// Index of the resrouce in the resrouce manager.
		IResource* rsc;

		
		// Private Contruct.
		ResourceRef()
			: type(EResourceType::RT_None)
			, rsc(nullptr)
		{

		}


	public:
		// Contruct.
		ResourceRef(IResource* resource)
			: path(resource->path)
			, type(resource->type)
			, rsc(resource)
		{

		}

		// Copy Construct.
		ResourceRef(const ResourceRef& other)
			: path(other.path)
			, type(other.type)
			, rsc(other.rsc)
		{

		}

		// Create ResourceRef from input archive.
		template<typename Archive>
		static inline void Save(Archive& archive, IResource* resource)
		{
			if (resource)
			{
				const ResourceRef ref(resource);
				archive(EnumAsInt<const EResourceType>(ref.type), ref.path);
			}
			else
			{
				const ResourceRef invalidRef;
				archive(EnumAsInt<const EResourceType>(invalidRef.type), invalidRef.path);
			}
		}


		// Create ResourceRef from input archive.
		template<typename Archive>
		static inline ResourceRef Load(Archive& archive)
		{
			ResourceRef ref;
			archive(EnumAsInt<EResourceType>(ref.type), ref.path);

			return ref;
		}


		// Return the relative path to the Resource.
		inline const std::string& GetPath() const { return path; }

		// Return true if the Resource is loaded and valid.
		bool IsValid() const { return rsc != nullptr; }

		// Find or load the Resource. 
		Ptr<IResource> FindOrLoad();

		// Return the resrouce if previously found.
		Ptr<IResource> GetResrouce();

		// Template FindOrLoad.
		template<class TResource>
		Ptr<TResource> FindOrLoad() 
		{ 
			Ptr<IResource> rsc = FindOrLoad();
			RAVEN_ASSERT(!rsc || TResource::StaticGetType() == rsc->GetType(), "Type Mismatch");
			return std::static_pointer_cast<TResource, IResource>(rsc);
		}

		// Template GetResrouce.
		template<class TResource>
		Ptr<TResource> GetResrouce()
		{
			Ptr<IResource> rsc = GetResrouce();
			RAVEN_ASSERT(!rsc || TResource::StaticGetType() == rsc->GetType(), "Type Mismatch");
			return std::static_pointer_cast<TResource, IResource>(rsc);
		}

	};


}