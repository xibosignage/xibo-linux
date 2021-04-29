project(layout)

add_library(${PROJECT_NAME}
    LayoutsManager.cpp
    LayoutsManager.hpp
    MainLayout.hpp
    MainLayoutImpl.cpp
    MainLayoutImpl.hpp
    MainLayoutOptions.hpp
    MainLayoutParser.cpp
    MainLayoutParser.hpp
    MainLayoutResources.hpp
    OverlayLayoutParser.hpp
)

target_link_libraries(${PROJECT_NAME}
    widgets
    stat
)
