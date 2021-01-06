project(xibo-watchdog)

find_package(Boost 1.70 EXACT REQUIRED program_options)

add_executable(${PROJECT_NAME}
    main.cpp
    ProcessWatcher.cpp
    ProcessWatcher.hpp
)

if (SNAP_BUILD OR APPIMAGE_BUILD)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
endif()

target_link_libraries(${PROJECT_NAME}
    config
    Boost::program_options
)
