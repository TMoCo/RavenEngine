//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <entt/entt.hpp>
namespace Raven 
{
	//TODO serialize function is not implementation
	struct NameComponent
	{
		template<typename Archive>
		void serialize(Archive& archive)
		{
			archive(cereal::make_nvp("Name", name));
		}
		std::string name;
	};


	struct ActiveComponent
	{
		template<typename Archive>
		void serialize(Archive& archive)
		{
			archive(cereal::make_nvp("Active", active));
		}
		bool active = true;
	};

	
	//Component - 
	class Hierarchy
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
			archive(cereal::make_nvp("First", first), cereal::make_nvp("Next", next), cereal::make_nvp("Previous", prev), cereal::make_nvp("Parent", parent));
		}
	};

};