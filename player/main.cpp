#include "XiboApp.hpp"

#include "common/logger/Logging.hpp"

int main(int /*argc*/, char** /*argv*/)
{
    try
    {
        auto&& app = XiboApp::create("org.gtkmm.xibo");

        return app.run();
    }
    catch (std::exception& e)
    {
        Log::error("[Main] {}", e.what());
    }

    return 0;
}
