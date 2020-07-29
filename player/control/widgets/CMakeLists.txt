project(widgets)

add_library(${PROJECT_NAME}
    Container.hpp
    FixedContainer.hpp
    FixedContainerFactory.cpp
    Image.hpp
    ImageWidgetFactory.cpp
    KeyboardKey.cpp
    KeyboardKey.hpp
    NativeWindow.hpp
    OutputWindow.hpp
    OverlayContainer.hpp
    OverlayContainerFactory.cpp
    SingleContainer.hpp
    StatusScreen.hpp
    StatusScreenFactory.hpp
    Widget.hpp
    Window.hpp
)

if(GTK_UI)
    target_link_libraries(${PROJECT_NAME} widgets_gtk)
    add_subdirectory(gtk)
endif(GTK_UI)
