#pragma once

#include <Reme/Core.h>
#include <Reme/Events/Event.h>

#include <functional>
#include <string>

namespace Reme {

using EventCallbackFn = std::function<void(Event)>;

class LIB_API Window {
public:
    static OwnPtr<Window> create(
        std::string title = "Unnamed Window",
        u32 width = 800,
        u32 height = 600);

    virtual ~Window() {}

    virtual u32 width() const = 0;
    virtual u32 height() const = 0;
    virtual void resize(u32 width, u32 height) = 0;

    virtual void set_title(const std::string& title) = 0;
    virtual std::string_view title() const = 0;

    virtual void* native_window() const = 0;
    virtual void set_event_callback(const EventCallbackFn&) = 0;
    virtual void poll_event() = 0;

    virtual void set_vsync(bool enabled) = 0;
    virtual bool vsync() const = 0;

    virtual void swap_buffer() = 0;
    virtual void make_context_current() = 0;
};

} // namespace Reme