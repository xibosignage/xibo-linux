#include "GlobalTestEnvironment.hpp"

int main(int argc, char *argv[])
{    
    ::testing::InitGoogleTest(&argc, argv);

    ::testing::AddGlobalTestEnvironment(new GlobalTestEnvironment);

    return RUN_ALL_TESTS();
}
