#pragma once

#include "Events.h"
#include <ASL/Forward.h>
#include <ASL/Function.h>
#include <ASL/RefCounted.h>
#include <ASL/RefPtr.h>
#include <ASL/UtilityStruct.h>

namespace GUI {

class LIB_API Window : public RefCounted<Window> {
public:
    static RefPtr<Window> construct(int x, int y, int width, int height, const char* title);

    virtual ~Window() {}

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

} // namespace GUI
