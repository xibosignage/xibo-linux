project(screenshot)

add_library(${PROJECT_NAME}
    ScreenShoter.cpp
    ScreenShoter.hpp
    ScreenShotInterval.cpp
    ScreenShotInterval.hpp
    ScreeShoterFactory.hpp
)

target_link_libraries(${PROJECT_NAME}
    common
)

if(GTK_UI)
    target_link_libraries(${PROJECT_NAME} screenshot_gtk)
    add_subdirectory(gtk)
endif(GTK_UI)
