#include "XiboApp.hpp"

int main(int argc, char** argv)
{
    auto&& app = XiboApp::create("org.gtkmm.xibo");

    return app.run(argc, argv);
}
