project(config)

if (NOT SNAP_BUILD)
    find_package(Git REQUIRED)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        RESULT_VARIABLE SHORT_HASH_RESULT
        OUTPUT_VARIABLE SHORT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(RESULT)
        message(FATAL_ERROR "Failed to get git hash: ${RESULT}")
    endif()
    message("Retrieved commit SHA ${SHORT_HASH}")
    configure_file(GitHash.hpp.in GitHash.hpp @ONLY)
endif()

add_library(${PROJECT_NAME}
    PlayerSettings.cpp
    PlayerSettings.hpp
    PlayerSettingsSerializer.cpp
    PlayerSettingsSerializer.hpp
    CmsSettings.cpp
    CmsSettings.hpp
    CmsSettingsSerializer.cpp
    CmsSettingsSerializer.hpp
    SettingsSerializer.hpp
    AppConfig.cpp
    AppConfig.hpp
    GitHash.hpp.in
)

target_link_libraries(${PROJECT_NAME}
    fs
    logger
    types
    common
    parsing
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)
