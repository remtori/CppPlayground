#pragma once

#include <Reme/Window.h>

typedef struct GLFWwindow GLFWwindow;

namespace Reme {

class GLFW_Window : public Window {
public:
    GLFW_Window(std::string title, u32 width, u32 height);
    virtual ~GLFW_Window();

    virtual u32 width() const override;
    virtual u32 height() const override;
    virtual void resize(u32 width, u32 height) override;

    virtual void set_title(const std::string& title) override;
    virtual std::string_view title() const override;

    virtual void* native_window() const override { return m_window; }

    virtual void set_event_callback(const EventCallbackFn& cb) override
    {
        m_data.event_callback = cb;
    }

    virtual void set_vsync(bool enabled) override;
    virtual bool vsync() const override;

    virtual void poll_event() override;
    virtual void swap_buffer() override;
    virtual void make_context_current() override;

private:
    GLFWwindow* m_window;

    struct WindowData {
        bool vsync;
        uint32_t width, height;
        std::string title;
        EventCallbackFn event_callback;
    };

    WindowData m_data;
};

} // namespace Reme