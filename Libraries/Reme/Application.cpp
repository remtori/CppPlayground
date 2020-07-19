#include <Reme/Application.h>

#include <Reme/Window.h>
#include <chrono>

namespace Reme {

Application* Application::s_instance;

Application::Application()
{
    ASSERT(!s_instance);
    s_instance = this;
}

Application::~Application()
{
}

void Application::run()
{
    float DeltaTime = 1000.0 / 60;
    auto win = Window::create();
    using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;

    TimeStamp currTime;
    TimeStamp lastTime = std::chrono::high_resolution_clock::now();
    float elapsedTime;

    while (true) {
        win->poll_event();

        currTime = std::chrono::high_resolution_clock::now();
        elapsedTime = std::chrono::duration<float>(currTime - lastTime).count();
        while (elapsedTime >= DeltaTime) {
            // update
            elapsedTime -= DeltaTime;
        }
        lastTime = currTime;

        win->swap_buffer();
    }
}

} // namespace Reme