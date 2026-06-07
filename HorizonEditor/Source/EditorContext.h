#pragma once

#include "Horizon/Core/Base.h"
#include "Horizon/Scene/Scene.h"
#include "Horizon/ECS/Entity.h"

#include <filesystem>

namespace Horizon
{
    struct EditorContext
    {
        Ref<Scene> ActiveScene;

        Entity SelectedEntity;

        std::filesystem::path ProjectPath;

        bool IsPlaying = false;
    };

    extern EditorContext g_EditorContext;
}