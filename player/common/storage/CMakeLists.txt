project(storage)

add_library(${PROJECT_NAME}
    FileCacheImpl.cpp
    FileCacheImpl.hpp
    FileCache.hpp
    RequiredItems.cpp
    RequiredItems.hpp
)

target_link_libraries(${PROJECT_NAME}
    dt
    fs
    logger
    crypto
    common
    parsing
)
