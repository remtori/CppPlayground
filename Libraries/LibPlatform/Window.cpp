#include "Window.h"

#include <ASL/Platform.h>

#ifdef PLATFORM_LINUX
#    include "X11Window.h"
#endif

namespace CrossPlatform {

Window::Window(int x, int y, int width, int height, const char* title)
{
#ifdef PLATFORM_LINUX
    m_window = new internal::X11Window(x, y, width, height, title);
#endif
}

} // namespace CrossPlatform
