#pragma once

#include "Horizon/Core/Base.h"
#include "Horizon/Core/KeyCodes.h"
#include "Horizon/Core/MouseCodes.h"
#include <utility>

namespace Horizon {

    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);
        static bool IsMouseButtonPressed(MouseCode button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };

} // namespace Horizon
