#include <LibGUI/Application.h>
#include <LibGUI/Window.h>

int main(int argc, char** argv)
{
    GUI::Application app(argc, argv);
    auto window = GUI::Window::construct(100, 100, 480, 600, "Hello!");

    while (1) {
        window->poll_events();
    }

    return 0;
}
