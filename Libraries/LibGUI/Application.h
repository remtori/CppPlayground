#pragma once

#include <ASL/Assertions.h>
#include <ASL/Forward.h>
#include <LibGUI/Events.h>

namespace GUI {

class LIB_API Application {
public:
    static Application& get()
    {
        ASSERT(s_instance);
        return *s_instance;
    }

    Application(int argc, char** argv);
    ~Application();

    void* app_data() const { return m_application_data; }
    void on_event(Event);

private:
    static Application* s_instance;

    void* m_application_data = nullptr;
};

} // namespace GUI
