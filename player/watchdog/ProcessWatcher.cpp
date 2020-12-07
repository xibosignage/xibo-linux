#include "ProcessWatcher.hpp"

ProcessWatcher::ProcessWatcher(const std::string& processBin, bool disableRestart) :
    processPath_{processBin},
    disableRestart_{disableRestart}
{
    setupSignalHandler();
}

void ProcessWatcher::run()
{
    while (true)
    {
        boost::process::child child{processPath_};
        currentChildPid = child.id();
        child.wait();
        std::cerr << "Player exited with code " << child.exit_code() << std::endl;

        if (terminate || disableRestart_) break;
        sleep(3);
    }
}

void ProcessWatcher::signalHandler(int signum)
{
    if (signum == SIGTERM)
    {
        kill(currentChildPid, SIGTERM);
        terminate = true;
    }
}

void ProcessWatcher::setupSignalHandler()
{
    struct sigaction action;
    action.sa_handler = signalHandler;
    sigaction(SIGTERM, &action, nullptr);
}
