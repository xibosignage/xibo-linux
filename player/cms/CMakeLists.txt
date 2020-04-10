project(cms)

add_library(${PROJECT_NAME}
    XmdsCommand.hpp
    RegisterDisplayCommand.cpp
    RegisterDisplayCommand.hpp
    RequiredFilesCommand.cpp
    RequiredFilesCommand.hpp
    GetScheduleCommand.cpp
    GetScheduleCommand.hpp
    CollectionInterval.cpp
    CollectionInterval.hpp
    RequiredFilesDownloader.cpp
    RequiredFilesDownloader.hpp
    CmsStatus.hpp
)

target_link_libraries(${PROJECT_NAME}
    networking
    stat
    schedule
    crypto
    xmr
    xmds
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${GTKMM_INCLUDE_DIRS} # TODO remove
)

add_subdirectory(xmds)
