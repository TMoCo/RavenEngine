//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Scene/Entity/Entity.h"
#include "Utilities/Core.h"
namespace Raven
{
	template<typename T>
	class EntityView
	{
		class iterator;
	public:
		EntityView(Scene* scene)
			: scene(scene), view(scene->GetRegistry().view<T>())
		{
		}

		inline auto operator[](int32_t i) { RAVEN_ASSERT(i < view.size(), "Index out of range on Entity View"); return Entity(view[i], scene); }

		inline auto Empty() const { return view.empty(); }
		inline auto Size() const { return view.size(); }
		inline auto Front() { return Entity(view[0], scene); }

	private:
		class iterator : public std::iterator<std::output_iterator_tag, Entity>
		{
		public:
			explicit iterator(EntityView<T>& view, size_t index = 0)
				:view(view)
				, nIndex(index)
			{
			}

			Entity operator*() const
			{
				return view[int32_t(nIndex)];
			}
			iterator& operator++()
			{
				nIndex++;
				return *this;
			}
			iterator operator++(int)
			{
				return ++(*this);
			}
			bool operator!=(const iterator& rhs) const
			{
				return nIndex != rhs.nIndex;
			}

		private:
			size_t nIndex = 0;
			EntityView<T>& view;
		};

		Scene* scene = nullptr;
		entt::basic_view<entt::entity, entt::exclude_t<>, T> view;
	public:
		iterator begin();
		iterator end();
	};


	template<typename T>
	typename EntityView<T>::iterator EntityView<T>::begin()
	{
		return EntityView<T>::iterator(*this, 0);
	};

	template<typename T>
	typename EntityView<T>::iterator EntityView<T>::end()
	{
		return EntityView<T>::iterator(*this, Size());
	};



	template<typename... Components>
	class EntityGroup
	{
	public:
		EntityGroup(Scene* scene)
			: scene(scene)
			, group(scene->GetRegistry().group<Components...>())
		{
		}

		inline auto operator[](int i) { RAVEN_ASSERT(i < Size(), "Index out of range on Entity View"); return Entity(group[i], scene); }
		inline auto Size() const { return group.size(); }
		inline auto Front() { return Entity(group[0], scene); }
	private:
		Scene* scene = nullptr;
		entt::group<Components...> group;
	};
};
