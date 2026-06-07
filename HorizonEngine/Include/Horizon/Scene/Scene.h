#pragma once

#include "Horizon/Core/Base.h"
#include "Horizon/Core/Timestep.h"
#include <entt/entt.hpp>

namespace Horizon {

    class Entity;
    class OrthographicCamera;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = "Empty Entity");
        void DestroyEntity(Entity entity);

        void OnUpdateRuntime(Timestep ts);
        void OnUpdateEditor(Timestep ts, const OrthographicCamera& camera);
        void OnViewportResize(uint32_t width, uint32_t height);

        Ref<Scene> Clone() const; // No-op here; implementation will be added in Scene.cpp

        template<typename... Components>
        auto GetAllEntitiesWith()
        {
            return m_Registry.view<Components...>();
        }

    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity;
        friend class SceneHierarchyPanel;
        friend class InspectorPanel;
    };

} // namespace Horizon
