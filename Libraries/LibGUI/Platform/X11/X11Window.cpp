#include <LibGUI/Platform/X11/X11Window.h>

#include <LibGUI/Application.h>
#include <LibGUI/Input.h>
#include <LibGUI/Platform/X11/X11Init.h>

namespace GUI {

namespace internal {

X11Window::X11Window(int x, int y, int width, int height, const char* title)
{
    X11Data* data = (X11Data*)Application::get().app_data();
    ASSERT(data);

    m_x11_window = XCreateSimpleWindow(
        data->display,
        RootWindow(data->display, data->screen),
        x, y, width, height,
        1,
        BlackPixel(data->display, data->screen),
        WhitePixel(data->display, data->screen));

    set_title(title);

    XSelectInput(
        data->display, m_x11_window,
        ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask
            | PointerMotionMask);
    XMapWindow(data->display, m_x11_window);
}

X11Window::~X11Window()
{
    XUnmapWindow(((X11Data*)Application::get().app_data())->display, m_x11_window);
}

void X11Window::set_title(const char* title)
{
    X11Data* data = (X11Data*)Application::get().app_data();
    XStoreName(data->display, m_x11_window, title);
    m_title = title;
}

void X11Window::set_window_rect(Rect r)
{
    if (r.x == -1)
        r.x = m_rect.x;
    if (r.y == -1)
        r.y = m_rect.y;
    if (r.width == -1)
        r.width = m_rect.width;
    if (r.height == -1)
        r.height = m_rect.height;

    XMoveResizeWindow(((X11Data*)Application::get().app_data())->display, m_x11_window, r.x, r.y, r.width, r.height);
}

void X11Window::process_event(XEvent& event)
{
    Application& app = Application::get();
    X11Data* data = (X11Data*)app.app_data();

    switch (event.type) {
    case Expose: {
        auto gc = DefaultGC(data->display, data->screen);
        u8* pixels = new u8[200 * 200 * 4];
        for (int i = 0; i < 200 * 200; ++i) {
            pixels[i * 4 + 0] = i % 255;
            pixels[i * 4 + 1] = 0;
            pixels[i * 4 + 2] = i % 128;
            pixels[i * 4 + 3] = 255;
        }

        Visual* visual = DefaultVisual(data->display, data->screen);

        XImage* img = XCreateImage(
            data->display,
            visual,
            DefaultDepth(data->display, data->screen),
            ZPixmap, 0,
            (char*)pixels, 200, 200, 32, 0);

        XPutImage(data->display, m_x11_window, gc, img, 0, 0, 0, 0, 200, 200);
        break;
    }
    case KeyPress: {
        KeyCode keycode = translate_keycode(event.xkey.keycode);
        int mods = translate_state(event.xkey.state);
        KeyDownEvent key_event(keycode, mods);
        key_event.set_event_source(this);
        app.on_event(key_event);
        break;
    }

    case KeyRelease: {
        // Handle Repeated key
        if (XEventsQueued(data->display, QueuedAfterReading)) {
            XEvent next_event;
            XPeekEvent(data->display, &next_event);

            if (next_event.type == KeyPress
                && next_event.xkey.time == event.xkey.time
                && next_event.xkey.keycode == event.xkey.keycode) {
                // Ignore repeated key release
                return;
            }
        }

        KeyCode keycode = translate_keycode(event.xkey.keycode);
        int mods = translate_state(event.xkey.state);
        KeyUpEvent key_event(keycode, mods);
        key_event.set_event_source(this);
        app.on_event(key_event);
        break;
    }

    case ButtonPress:
        break;
    case ButtonRelease:
        break;
    }
}

void X11Window::poll_events()
{
    X11Data* data = (X11Data*)Application::get().app_data();
    XPending(data->display);

    while (QLength(data->display)) {
        XEvent event;
        XNextEvent(data->display, &event);
        process_event(event);
    }

    XFlush(data->display);
}

} // namespace internal

} // namespace GUI
