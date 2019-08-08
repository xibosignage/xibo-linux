#include <boost/process/child.hpp>
#include <iostream>

#include "common/fs/FileSystem.hpp"
#include "config.hpp"

int main()
{
    if(FileSystem::exists(ProjectResources::cmsSettingsFile()))
    {
        boost::process::child playerBin{ProjectResources::playerBinary()};

        playerBin.wait();
    }
    else
    {
        boost::process::child optionsBin{ProjectResources::optionsBinary()};

        optionsBin.wait();
    }
    return 0;
}
