project(watchdog)

add_executable(${PROJECT_NAME}
    main.cpp
    ProcessWatcher.cpp
    ProcessWatcher.hpp
)
target_link_libraries(${PROJECT_NAME}
    config
)
