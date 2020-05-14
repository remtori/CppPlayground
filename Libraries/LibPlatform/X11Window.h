#pragma once

#include "Window.h"
#include <ASL/String.h>
#include <X11/Xlib.h>

namespace CrossPlatform {

namespace internal {

class X11Window final : public AbstractWindow {
public:
    X11Window(int x, int y, int width, int height, const char* title);
    ~X11Window();

    void set_title(const char*) override;
    const char* title() const override { return m_title.characters(); }

    void set_window_rect(Rect) override;
    Rect rect() const override { return m_rect; }

    void poll_events() override;

private:
    void process_event(XEvent&);

    Rect m_rect;
    String m_title;
    int m_x11_screen;
    ::Display* m_x11_display;
    ::Window m_x11_window;
};

} // namespace internal

} // namespace CrossPlatform
