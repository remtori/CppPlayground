#pragma once

#include "Events.h"

namespace CrossPlatform {

struct Rect {
    int x = -1;
    int y = -1;
    int width = -1;
    int height = -1;
};

class AbstractWindow {
public:
    virtual ~AbstractWindow() {}

    virtual void set_title(const char*) = 0;
    virtual const char* title() const = 0;

    virtual void set_window_size(int width, int height) { set_window_rect({ -1, -1, width, height }); }
    virtual int width() const { return rect().width; }
    virtual int height() const { return rect().height; }

    virtual void set_window_position(int x, int y) { set_window_rect({ x, y }); }
    virtual int x() const { return rect().x; };
    virtual int y() const { return rect().y; };

    virtual void set_window_rect(Rect) = 0;
    virtual Rect rect() const = 0;

    virtual void poll_events() = 0;
    // virtual void wait_events() = 0;
    // virtual void wait_events_timeout(double timeout) = 0;

protected:
    struct WindowInputState {
        float scroll_x;
        float scroll_y;
    } m_input_state;
};

class Window {
public:
    Window(int x, int y, int width, int height, const char* title);
    ~Window()
    {
        if (m_window)
            delete m_window;
    }

    inline void set_title(const char* title) { m_window->set_title(title); };
    inline const char* title() const { return m_window->title(); }

    inline void set_window_size(int width, int height) { m_window->set_window_size(width, height); }
    inline int width() const { return m_window->width(); }
    inline int height() const { return m_window->height(); }

    inline void set_window_position(int x, int y) { m_window->set_window_position(x, y); }
    inline int x() const { return m_window->x(); }
    inline int y() const { return m_window->y(); }

    inline void set_window_rect(Rect r) { m_window->set_window_rect(r); }
    inline Rect rect() const { return m_window->rect(); }

    inline void poll_events() { m_window->poll_events(); }
    // inline void wait_events() { m_window->wait_events(); }
    // inline void wait_events_timeout(double timeout) { m_window->wait_events_timeout(timeout); }

private:
    AbstractWindow* m_window = nullptr;
};

} // namespace CrossPlatform
