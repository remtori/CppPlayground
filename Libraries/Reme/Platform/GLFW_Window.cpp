#include <Reme/Platform/GLFW_Window.h>

#include <ASL/Assert.h>
#include <ASL/Logger.h>
#include <GLFW/glfw3.h>

namespace Reme {

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
    LOG_ERROR("GLFW Error ({}): {}", error, description);
}

GLFW_Window::GLFW_Window(std::string title, u32 width, u32 height)
{
    m_data.title = title;
    m_data.width = width;
    m_data.height = height;

    LOG_INFO("Creating window {} ({}, {})", title, width, height);

    if (s_GLFWWindowCount == 0) {
        int success = glfwInit();
        ASSERT(success);
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    s_GLFWWindowCount++;

    if (!m_window) {
        glfwTerminate();
        LOG_ERROR("Failed to create GLFW window!");
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(m_window, &m_data);

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
        data->width = width;
        data->height = height;

        // WindowResizeEvent e(width, height);
        // data->EventCallback(e);
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
        // WindowCloseEvent event;
        // data->EventCallback(event);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
            // KeyDownEvent event(static_cast<KeyCode>(key), 0);
            // data->EventCallback(event);
            break;
        }

        case GLFW_RELEASE: {
            // KeyUpEvent event(static_cast<KeyCode>(key));
            // data->EventCallback(event);
            break;
        }

        case GLFW_REPEAT: {
            // KeyDownEvent event(static_cast<KeyCode>(key), 1);
            // data->EventCallback(event);
            break;
        }
        }
    });

    glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
        // KeyPressEvent event(static_cast<KeyCode>(keycode));
        // data->EventCallback(event);
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
        // MouseMoveEvent event((float)xpos, (float)ypos);
        // data->EventCallback(event);
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
            // MouseDownEvent event(static_cast<MouseCode>(button));
            // data->EventCallback(event);
            break;
        }
        case GLFW_RELEASE: {
            // MouseUpEvent event(static_cast<MouseCode>(button));
            // data->EventCallback(event);
            break;
        }
        }
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

        // MouseScrollEvent event((float)xOffset, (float)yOffset);
        // data->EventCallback(event);
    });

    make_context_current();
}

GLFW_Window::~GLFW_Window()
{
    glfwDestroyWindow(m_window);
    s_GLFWWindowCount--;

    if (s_GLFWWindowCount == 0) {
        glfwTerminate();
    }
}

void GLFW_Window::poll_event()
{
    glfwPollEvents();
}

void GLFW_Window::swap_buffer()
{
    glfwSwapBuffers(m_window);
}

void GLFW_Window::make_context_current()
{
    glfwMakeContextCurrent(m_window);
}

uint32_t GLFW_Window::width() const
{
    return m_data.width;
}

uint32_t GLFW_Window::height() const
{
    return m_data.height;
}

void GLFW_Window::resize(u32 width, u32 height)
{
    m_data.width = width;
    m_data.height = height;
    glfwSetWindowSize(m_window, width, height);
}

std::string_view GLFW_Window::title() const
{
    return m_data.title;
}

void GLFW_Window::set_title(const std::string& title)
{
    m_data.title = title;
    glfwSetWindowTitle(m_window, title.c_str());
}

void GLFW_Window::set_vsync(bool enabled)
{
    m_data.vsync = enabled;

    if (enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}

bool GLFW_Window::vsync() const
{
    return m_data.vsync;
}

} // namespace Reme