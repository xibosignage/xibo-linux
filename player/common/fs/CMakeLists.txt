project(fs)

add_library(${PROJECT_NAME}
    FilePath.hpp
    FileSystem.cpp
    FileSystem.hpp
    Resource.hpp
    StorageUsageInfo.hpp
)

target_link_libraries(${PROJECT_NAME}
    types
    config
    Boost::filesystem
)
