#include "XiboApp.hpp"

#include "common/logger/Logging.hpp"

#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
    try
    {
        auto&& app = XiboApp::create("org.gtkmm.xibo");

        auto var = getenv("GST_PLUGIN_PATH");
        if (var)
        {
            std::cout << "GST_PLUGIN_PATH " << var << std::endl;
        }
        var = getenv("GST_PLUGIN_SYSTEM_PATH");
        if (var)
        {
            std::cout << "GST_PLUGIN_SYSTEM_PATH " << var << std::endl;
        }
        var = getenv("GST_PLUGIN_SCANNER");
        if (var)
        {
            std::cout << "GST_PLUGIN_SCANNER " << var << std::endl;
        }

        return app.run();
    }
    catch (std::exception& e)
    {
        Log::error("[Main] {}", e.what());
    }

    return 0;
}
