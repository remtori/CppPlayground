#include <LibGUI/Window.h>

#include <ASL/Platform.h>

#ifdef PLATFORM_LINUX
#    include <LibGUI/Platform/X11/X11Window.h>
#endif

namespace GUI {

RefPtr<Window> Window::construct(int x, int y, int width, int height, const char* title)
{
#ifdef PLATFORM_LINUX
    return adopt(*new internal::X11Window(x, y, width, height, title));
#endif

    return nullptr;
}

} // namespace GUI
