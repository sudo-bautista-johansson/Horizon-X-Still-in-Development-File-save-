#pragma once

#include "Horizon/Core/Base.h"

namespace Horizon {

    enum class SceneState
    {
        Edit = 0,
        Play = 1,
        Pause = 2
    };

    class Toolbar
    {
    public:
        Toolbar();

        void OnImGuiRender();

        SceneState GetState() const { return m_State; }
        void SetState(SceneState state) { m_State = state; }

    private:
        SceneState m_State = SceneState::Edit;
    };

} // namespace Horizon