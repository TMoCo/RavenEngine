//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <entt/entt.hpp>
namespace Raven 
{
	//TODO serialize function is not implementation

	class Component 
	{
	public:
		virtual ~Component() = default;
		entt::entity entity = entt::null;

	};

	class NameComponent : public Component
	{
	public:
		NameComponent() = default;

		NameComponent(const std::string & name) :name(name) {}

		template<typename Archive>
		void serialize(Archive& archive)
		{
			archive(cereal::make_nvp("Name", name), cereal::make_nvp("Id", entity));
		}
		std::string name;
	};


	class ActiveComponent : public Component
	{
	public:
		ActiveComponent() = default;
		ActiveComponent(bool active) :active(active) {}
		template<typename Archive>
		void serialize(Archive& archive)
		{
			archive(cereal::make_nvp("Active", active), cereal::make_nvp("Id", entity));
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

		template<typename Archive>
		void serialize(Archive& archive)
		{
			archive(cereal::make_nvp("First", first), cereal::make_nvp("Next", next), cereal::make_nvp("Previous", prev), cereal::make_nvp("Parent", parent), cereal::make_nvp("Id", entity));
		}
	};

};