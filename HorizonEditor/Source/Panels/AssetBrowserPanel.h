#pragma once

#include "Horizon/Core/Base.h"
#include <filesystem>

namespace Horizon {

    class AssetBrowserPanel
    {
    public:
        AssetBrowserPanel();
        
        void OnImGuiRender();

    private:
        std::filesystem::path m_CurrentDirectory;
    };

} // namespace Horizon
