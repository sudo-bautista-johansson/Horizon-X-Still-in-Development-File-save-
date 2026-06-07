#pragma once

#include "Horizon/Core/Layer.h"
#include "Horizon/Core/Events.h"

namespace Horizon {

    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& e) override;

        void Begin();
        void End();

        void SetBlockEvents(bool block) { m_BlockEvents = block; }

    private:
        bool m_BlockEvents = true;
    };

} // namespace Horizon
