project(widgets_gtk)

add_library(${PROJECT_NAME}
    FixedContainerGtk.hpp
    ImageGtk.cpp
    ImageGtk.hpp
    OutputWindowGtk.cpp
    OutputWindowGtk.hpp
    OverlayContainerGtk.hpp
    StatusScreenGtk.cpp
    StatusScreenGtk.hpp
    WidgetGtk.hpp
    WindowGtk.cpp
    WindowGtk.hpp
)

target_link_libraries(${PROJECT_NAME}
    ${GTKMM_LINK_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${GTKMM_INCLUDE_DIRS}
)
