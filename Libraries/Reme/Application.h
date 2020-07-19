#pragma once

#include <ASL/Assert.h>
#include <ASL/Forward.h>

namespace Reme {

class LIB_API Application {
private:
    static Application* s_instance;

public:
    Application();
    ~Application();

    Application& the()
    {
        ASSERT(s_instance);
        return *s_instance;
    }

    void run();

protected:
private:
};

} // namespace Reme