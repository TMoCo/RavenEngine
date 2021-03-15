## ECS Architecture

#### [Entity](./Entity/Entity.h)


It 's a wrapper of entt::entity

#### [EntityManager](./Entity/EntityManager.h)

EntityManager manage a scene's all entites. In fact, it is a wrapper of entt::registry.


#### [EntityFactory](./Entity/EntityFactory.h)

Create some pre-defined entites

TODO


#### Use case 

how to iterate entities with components

##### Basic usage for entt::view
``` c++
    auto & registry = scene->GetRegistry();  
    //get all entities without Hierarchy component
    auto nonHierarchyView = registry.view<Transform>(entt::exclude<Hierarchy>);
    //get all entities which have Transform and Hierarchy components
    auto view = registry.view<Transform, Hierarchy>();

    for(auto entity: view) {
        // a component at a time ...
        auto &transform = view.get<Transform>(entity);
        auto &hierarchy = view.get<Hierarchy>(entity);
        // ... multiple components ...
        auto [trans, hier] = view.get<Transform, Hierarchy>(entity);
        // ... all components at once
        auto [trans, hier] = view.get(entity);
        // ...
    }

```

##### Basic usage for entt::group

Groups are meant to iterate multiple components at once and to offer a faster alternative to multi component views

``` c++
    //Model is a kind of Entity.
    //So, in here. We can get all entites which have Model and Transform
    auto group = registry.group<Model>(entt::get<Transform>);
    for(auto entity : group)
    {
        const auto& [model, trans] = group.get<Model, Transform>(entity);

        const auto& meshes = model.GetMeshes();

        for(auto mesh : meshes)
        {
            if(mesh->GetActive())
            {
                //do you job
                SubmitMesh();
            }
        }
    }

```


#### [Scene](./SceneManager.h)

**No-Copyable**

Contains a unique sceneGraph and entityManager. 
 
Important methods :

 **Serialize(const std::string& )**  
 **Load(const std::string& )** 



#### [SceneManager](./SceneManager.h)

It is not **copyable** and manages all scenes at runtime.

the most important method is **GetCurrentScene() const** which will returen the current scene. the scene is not null.


#### [SceneGraph](./SceneGraph.h)

Organize entities and decide which entity will be updated.


### Components 

##### [Active](./Component/Component.h)
##### [Name](./Component/Component.h)
##### [Hierarchy](./Component/Component.h)

##### [Light](./Component/Light.h)
##### [Model](./Component/Model.h)
##### [Transform](./Component/Transform.h)

