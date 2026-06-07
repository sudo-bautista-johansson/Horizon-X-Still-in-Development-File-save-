#include "Horizon/ECS/Entity.h"

namespace Horizon {

    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }

} // namespace Horizon
