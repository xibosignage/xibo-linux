project(screenshot_gtk)

pkg_check_modules(X11 REQUIRED x11)

add_library(${PROJECT_NAME}
    X11ScreenShoter.cpp
    X11ScreenShoter.hpp
)

target_link_libraries(${PROJECT_NAME}
    widgets_gtk
    ${X11_LINK_LIBRARIES}
)
