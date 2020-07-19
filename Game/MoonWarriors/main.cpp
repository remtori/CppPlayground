#include <ASL/Init.h>
#include <Reme/Application.h>

class GameApp : public Reme::Application {
};

int main(int argc, char** argv)
{
    ASL::initialize(argv[0]);
    GameApp app;
    app.run();
}
