project(region)

add_library(${PROJECT_NAME}
    GetMediaPosition.cpp
    GetMediaPosition.hpp
    Region.hpp
    RegionImpl.cpp
    RegionImpl.hpp
    RegionOptions.hpp
    RegionParser.cpp
    RegionParser.hpp
    RegionResources.hpp
)

target_link_libraries(${PROJECT_NAME}
    widgets
)
