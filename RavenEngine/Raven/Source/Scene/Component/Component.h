//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <entt/entt.hpp>
namespace Raven 
{
	//TODO serialize function is not implementation
	class Entity;
	class Component 
	{
	public:
		virtual ~Component() = default;
		entt::entity entity = entt::null;
		Entity GetEntity() const;

		// serialize save.
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("Id", entity));
		}

		// serialize load.
		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("Id", entity));
		}
	};

	class NameComponent : public Component
	{
	public:
		NameComponent() = default;

		NameComponent(const std::string & name) :name(name) {}

		// serialize save.
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<Component>(this));
			archive(cereal::make_nvp("Name", name));
		}

		// serialize load.
		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<Component>(this));
			archive(cereal::make_nvp("Name", name));
		}

		// Component Name.
		std::string name;
	};


	class ActiveComponent : public Component
	{
	public:
		ActiveComponent() = default;
		ActiveComponent(bool active) :active(active) {}

		// serialize save.
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<Component>(this));
			archive(cereal::make_nvp("Active", active));
		}

		// serialize load.
		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<Component>(this));
			archive(cereal::make_nvp("Active", active));
		}


		bool active = true;
	};

	
	//Component - 
	class Hierarchy : public Component
	{
	public:
		Hierarchy(entt::entity p);
		Hierarchy();

		inline auto Parent() const { return parent; }
		inline auto Next() const { return next;}
		inline auto Prev() const  {return prev;}
		inline auto First() const {return first;}

		// Return true if current entity is an ancestor of current entity
		//seems that the entt as a reference could have a bug....
		//TODO 
		bool Compare(const entt::registry& registry, entt::entity entity) const;
		void Reset();


		//delegate method
		// update hierarchy components when hierarchy component is added
		static void OnConstruct(entt::registry& registry, entt::entity entity);
		// update hierarchy components when hierarchy component is removed
		static void OnDestroy(entt::registry& registry, entt::entity entity);
		static void OnUpdate(entt::registry& registry, entt::entity entity);

		//adjust the parent 
		static void Reparent(entt::entity entity, entt::entity parent, entt::registry& registry, Hierarchy& hierarchy);

		entt::entity parent = entt::null;
		entt::entity first = entt::null;
		entt::entity next = entt::null;
		entt::entity prev = entt::null;

		// serialize save.
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<Component>(this));
			archive(
				cereal::make_nvp("First", first),
				cereal::make_nvp("Next", next),
				cereal::make_nvp("Previous", prev),
				cereal::make_nvp("Parent", parent)
			);
		}

		// serialize load.
		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<Component>(this));
			archive(
				cereal::make_nvp("First", first),
				cereal::make_nvp("Next", next),
				cereal::make_nvp("Previous", prev),
				cereal::make_nvp("Parent", parent)
			);
		}
	};

};