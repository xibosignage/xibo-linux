project(fs)

add_library(${PROJECT_NAME}
    FileCache.hpp
    FileCacheImpl.cpp
    FileCacheImpl.hpp
    FilePath.hpp
    FileSystem.cpp
    FileSystem.hpp
    Resource.hpp
)

target_link_libraries(${PROJECT_NAME}
    types
    config
    logger
    Boost::filesystem
)
