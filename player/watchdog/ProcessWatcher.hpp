#pragma once

#include <boost/process/child.hpp>
#include <iostream>
#include <signal.h>

class ProcessWatcher
{
public:
    ProcessWatcher(const std::string& processBin);
    void run();

private:
    static void signalHandler(int signum);
    void setupSignalHandler();

private:
    static inline int currentChildPid = -1;
    static inline bool terminate = false;

private:
    std::string processPath_;
};
