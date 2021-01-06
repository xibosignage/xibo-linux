#include "XiboApp.hpp"
#include "common/logger/Logging.hpp"

#include <X11/Xlib.h>
#include <boost/stacktrace.hpp>
#include <iostream>
#include <signal.h>

/* DISCLAIMER
 * Boost provide boost::stacktrace::stacktrace::safe_dump_to which statisfies the signal-safety
 * requirements, however, it gives very unuseful stacktrace without function names/lines. So we
 * decide to go with unsafe stacktrace taking into account we call it in SIGSEGV and SIGABRT
 * signal handlers which are already the indicators of fatal error
 */
void signalStacktraceHandler(int signum)
{
    signal(signum, SIG_DFL);
    std::cout << boost::stacktrace::stacktrace() << std::endl;
    raise(signum);
}

int main(int /*argc*/, char** /*argv*/)
{
    std::cout << AppConfig::version() << std::endl;

    XInitThreads();
    signal(SIGSEGV, &signalStacktraceHandler);
    signal(SIGABRT, &signalStacktraceHandler);

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
