project(control)

add_library(${PROJECT_NAME}
    ApplicationWindow.hpp
    XlfResources.hpp
)

target_link_libraries(${PROJECT_NAME}
    screenshot
    media
    layout
    region
    widgets
    status
)

add_subdirectory(screenshot)
add_subdirectory(transitions)
add_subdirectory(layout)
add_subdirectory(region)
add_subdirectory(media)
add_subdirectory(widgets)
add_subdirectory(status)
