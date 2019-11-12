project(cms)

file(GLOB XMDS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/xmds/*.*pp
)

add_library(${PROJECT_NAME}
    CollectionInterval.cpp
    CollectionInterval.hpp
    RequiredFilesDownloader.cpp
    RequiredFilesDownloader.hpp
    CmsStatus.hpp
    ${XMDS_SOURCES}
)
target_link_libraries(${PROJECT_NAME}
    networking
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${GTKMM_INCLUDE_DIRS} # remove
)
