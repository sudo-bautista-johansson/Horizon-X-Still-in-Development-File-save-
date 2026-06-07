#pragma once

#include "Horizon/Core/Base.h"
#include "Horizon/Scene/Scene.h"
#include "Horizon/ECS/Entity.h"

namespace Horizon
{
    class SceneHierarchyPanel
    {
    public:

        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& context);

        void SetContext(const Ref<Scene>& context);

        void SyncSelectionToContext();

        void OnImGuiRender();

        Entity GetSelectedEntity() const
        {
            return m_SelectionContext;
        }

        void SetSelectedEntity(Entity entity)
        {
            m_SelectionContext = entity;
        }

    private:

        void DrawEntityNode(Entity entity);

    private:

        Ref<Scene> m_Context;

        Entity m_SelectionContext;

        bool m_Syncing = false;
    };
}