#include <Reme/Window.h>

#include <ASL/Platform.h>

#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)
#    include <Reme/Platform/GLFW_Window.h>
#else
#endif

namespace Reme {

OwnPtr<Window> Window::create(std::string title, u32 width, u32 height)
{
#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)
    return create_own<GLFW_Window>(title, width, height);
#else
#endif
}

} // namespace Reme