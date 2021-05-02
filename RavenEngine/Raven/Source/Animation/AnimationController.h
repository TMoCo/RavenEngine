//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <list>
#include <cereal/cereal.hpp>
#include <cereal/types/unordered_map.hpp>
#include <entt/entt.hpp>

#include "Utilities/StringUtils.h"
#include "Animation.h"

namespace Raven
{
	class Animation;
	class Scene;
	class SkinnedMeshComponent;

	struct Condition
	{
		enum class Type
		{
			Float,
			Int,
			Bool,
		};
		std::string value;
		Type type;

		inline operator bool() const {
			return std::stoi(value) == 1; 
		}

		inline bool operator ==(const Condition & con) const {
			return con.type == type && con.value == value;
		}

		inline bool operator !=(const Condition& con) const {
			return con.type != type || con.value != value;
		}

		inline operator int() const
		{
			return std::stoi(value);
		}

		inline operator float() const
		{
			return std::stof(value);
		}

		template <typename Archive>
		void serialize(Archive& archive)
		{
			archive(cereal::make_nvp("value", value));
			archive(cereal::make_nvp("type", type));
		}

	};

	struct Transition
	{
		int32_t id;
		int32_t from;
		int32_t to;
		std::unordered_map<std::string, Condition> conditions;

		template <typename Archive>
		void serialize(Archive& archive)
		{
			archive(cereal::make_nvp("id", id));
			archive(cereal::make_nvp("from", from));
			archive(cereal::make_nvp("to", to));
			archive(cereal::make_nvp("conditions", conditions));
		}

	};

	class AnimationController 
	{
	public:
		struct AnimatorNode
		{
			int32_t nodeId;
			int32_t in;//ma
			int32_t out;
			std::string name;//file Path
			std::string shortName;

			template <typename Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("in", in));
				archive(cereal::make_nvp("out", out));
				archive(cereal::make_nvp("nodeId", nodeId));
				archive(cereal::make_nvp("name", name));
				shortName = StringUtils::GetFileNameWithoutExtension(name);
			}

			template <typename Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("in", in));
				archive(cereal::make_nvp("out", out));
				archive(cereal::make_nvp("nodeId", nodeId));
				archive(cereal::make_nvp("name", name));

			}
		};
		constexpr static int32_t EntryNodeId = 100;

	public:
		AnimationController(const std::string & fileName);

		inline void FocusTransition(Transition * info) { focusedLink = info; }
		inline auto& GetTransition(int32_t linkId) { return linkInfo[linkId]; }
		inline auto& GetConditions() const { return conditions; }
		inline auto& GetAnimatorNodes() const { return animatorNodes; }
		inline auto& GetLinkInfo() const { return linkInfo; }
		inline auto& GetCurrentLink() const { return currentLink; }
		inline auto& GetCurrAnimation() const { return currentAnimation; }
		
		const std::string GetCurrentAnimationName() const;
		

		void RemoveCondition(const std::string& key);
		void ChangeConditionName(const std::string& old, const std::string& newName);
		void SetValue(const std::string& name, const std::string& value);

		void OnImGui();
		void AddCondition(Condition::Type type);

		void Connect(
			const std::string& fromName, int32_t fromId, int32_t fromIn,int32_t fromOut,
			const std::string& toName, int32_t toId, int32_t toIn, int32_t toOut, int32_t linkId);
		void RemoveLink(int32_t link);


		std::string GetName();
		inline auto& GetFileName() const { return fileName; }
		void Save();
		void Load();

		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("animatorNodes", animatorNodes));
			archive(cereal::make_nvp("linkInfo", linkInfo));
			archive(cereal::make_nvp("conditions", conditions));
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("animatorNodes", animatorNodes));
			archive(cereal::make_nvp("linkInfo", linkInfo));
			archive(cereal::make_nvp("conditions", conditions));

			for (auto & nodes : linkInfo)
			{	
				//first animation node
				//EntryNodeId means the Entry nodeId 
				if (nodes.second.from == EntryNodeId)
				{
					currentNodeId = nodes.second.to;
					currentLink = nodes.first;
				}
			}
			focusedLink = nullptr;
			LoadAnimation();
		}

		void OnUpdate(float dt, SkinnedMeshComponent* skinnedComp);


	private:

		void LoadAnimation();

		Transition * focusedLink = nullptr;

		std::unordered_map<int32_t,AnimatorNode> animatorNodes;
		std::unordered_map<int32_t, Transition> linkInfo;
		std::map<std::string, Condition> conditions;
		
		std::string fileName;

		//###runtime value
		int32_t currentNodeId = 0;
		int32_t currentLink = 0;
		Ptr<Animation> currentAnimation;


		std::unordered_map<int32_t, Transition>::iterator iterId;
	};

};