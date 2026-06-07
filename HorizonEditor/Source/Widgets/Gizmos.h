#pragma once

#include "Horizon/Core/Base.h"
#include "Horizon/ECS/Entity.h"
#include "Horizon/Renderer/OrthographicCamera.h"

namespace Horizon {

    enum class GizmoType
    {
        Translate = 0,
        Rotate = 1,
        Scale = 2
    };

    class Gizmos
    {
    public:
        static void Draw(Entity selectedEntity, const OrthographicCamera& camera, GizmoType type);
    };

} // namespace Horizon
