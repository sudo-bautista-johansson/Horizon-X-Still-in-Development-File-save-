#pragma once

#include <glm/glm.hpp>

namespace Horizon {

    class RenderCommand
    {
    public:
        static void SetClearColor(const glm::vec4& color);
        static void Clear();
        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    };

} // namespace Horizon
