project(xibo-options)

add_executable(${PROJECT_NAME}
    main.cpp
    MainWindowController.cpp
    MainWindowController.hpp
    Resources.hpp
)

target_link_libraries(${PROJECT_NAME}
    cms
    config
    system
    ${GTKMM_LINK_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${GTKMM_INCLUDE_DIRS}
)
