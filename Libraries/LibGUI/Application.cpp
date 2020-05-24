#include <LibGUI/Application.h>

#include <ASL/LogStream.h>
#include <ASL/Platform.h>

#ifdef PLATFORM_LINUX
#    include <LibGUI/Platform/X11/X11Init.h>
#endif

namespace GUI {

Application* Application::s_instance = nullptr;

Application::Application(int argc, char** argv)
{
    ASSERT(s_instance == nullptr);
    s_instance = this;

    ASL::set_process_name(argc, argv);
    internal::init(m_application_data);
}

Application::~Application()
{
}

void Application::on_event(Event event)
{
}

} // namespace GUI
