#pragma once

#include "Horizon/Core/Base.h"

namespace Horizon {

    class ConsolePanel
    {
    public:
        ConsolePanel();
        
        void OnImGuiRender();

    private:
        bool m_ShowInfo = true;
        bool m_ShowWarn = true;
        bool m_ShowError = true;
        bool m_AutoScroll = true;
        char m_SearchFilter[256] = { 0 };
    };

} // namespace Horizon
