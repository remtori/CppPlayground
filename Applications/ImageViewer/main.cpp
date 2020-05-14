#include <LibPlatform/Window.h>

int main()
{
    CrossPlatform::Window window(100, 100, 480, 600, "Hello!");
    while (1) {
        window.poll_events();
    }

    return 0;
}
