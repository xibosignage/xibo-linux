project(parsing)

add_library(${PROJECT_NAME}
    Parsing.cpp
    Parsing.hpp
    XmlFileLoaderSaver.cpp
    XmlFileLoaderSaver.hpp
    XmlFileLoader.hpp
    XmlFileSaver.hpp
)

target_link_libraries(${PROJECT_NAME}
    common
    fs
    logger
)
