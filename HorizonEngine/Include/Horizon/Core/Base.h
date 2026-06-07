#pragma once

#include <memory>
#include <string>
#include <functional>
#include <cstdint>

// ── Platform Detection ──────────────────────────────────────────────────────
#ifdef _WIN32
    #define HZ_PLATFORM_WINDOWS
#elif defined(__linux__)
    #define HZ_PLATFORM_LINUX
#elif defined(__APPLE__)
    #define HZ_PLATFORM_MACOS
#endif

// ── Debug Break ─────────────────────────────────────────────────────────────
#ifdef HZ_PLATFORM_WINDOWS
    #define HZ_DEBUGBREAK() __debugbreak()
#else
    #include <signal.h>
    #define HZ_DEBUGBREAK() raise(SIGTRAP)
#endif

// ── Assertions ──────────────────────────────────────────────────────────────
#ifdef HZ_ENABLE_ASSERTS
    #define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); HZ_DEBUGBREAK(); } }
    #define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); HZ_DEBUGBREAK(); } }
#else
    #define HZ_ASSERT(x, ...)
    #define HZ_CORE_ASSERT(x, ...)
#endif

// ── Bit Manipulation ────────────────────────────────────────────────────────
#define BIT(x) (1 << (x))

// ── Event Bind ──────────────────────────────────────────────────────────────
#define HZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// ── Profiling Stub ──────────────────────────────────────────────────────────
#define HZ_PROFILE_FUNCTION()
#define HZ_PROFILE_SCOPE(name)

// ── Smart Pointer Aliases ───────────────────────────────────────────────────
namespace Horizon {

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

#ifdef Q
#undef Q
#endif
#ifdef vec
#undef vec
#endif
#ifdef mat
#undef mat
#endif

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

} // namespace Horizon
