#include "XiboApp.hpp"

#include <iostream>

int main()
{
    try
    {
        auto&& app = XiboApp::create("org.gtkmm.xibo");

        return app.run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

}
