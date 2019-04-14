#include <boost/process/child.hpp>
#include <iostream>

#include "common/FileSystem.hpp"
#include "config.hpp"

int main()
{
    if(FileSystem::exists(DEFAULT_CMS_SETTINGS_FILE))
    {
        boost::process::child playerBin{PLAYER_EXE};

        playerBin.wait();
    }
    else
    {
        boost::process::child optionsBin{PLAYER_OPTIONS_EXE};

        optionsBin.wait();
    }
    return 0;
}
