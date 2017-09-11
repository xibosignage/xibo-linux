#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t child_id = fork();
    if(child_id > 0)
    {
        std::cout << child_id << std::endl;
        int status;
        std::cout << waitpid(-1, &status, 0) << std::endl;
        std::cout << "child process exited with status: " << status << std::endl;
    }
    else
    {
        if(child_id == 0)
        {
            char* args[] = {"./xibo-linux", NULL};
            std::cout << execv("./xibo-linux", args) << std::endl;
            std::cout << "now child" << errno << std::endl;
        }
        else
        {
            std::cout << "some error" << std::endl;
        }
    }
    return 0;
}
