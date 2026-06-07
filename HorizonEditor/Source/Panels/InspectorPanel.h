#pragma once

#include "Horizon/Core/Base.h"
#include "Horizon/ECS/Entity.h"

namespace Horizon {

    class InspectorPanel
    {
    public:
        InspectorPanel() = default;
        
        void OnImGuiRender(Entity selectedEntity);

    private:
        void DrawComponents(Entity entity);
    };

} // namespace Horizon
