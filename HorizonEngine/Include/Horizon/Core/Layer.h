#pragma once

#include "Horizon/Core/Base.h"
#include "Horizon/Core/Timestep.h"
#include "Horizon/Core/Events.h"
#include <string>

namespace Horizon {

    class Layer
    {
    public:
        Layer(const std::string& name = "Layer")
            : m_DebugName(name) {}
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };

} // namespace Horizon
