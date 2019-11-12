project(options)

add_executable(${PROJECT_NAME}
    main.cpp
    MainWindowController.cpp
    MainWindowController.hpp
    Resources.hpp
)
target_link_libraries(${PROJECT_NAME}
    cms
    config
    ${GTKMM_LINK_LIBRARIES}
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${GTKMM_INCLUDE_DIRS}
)
