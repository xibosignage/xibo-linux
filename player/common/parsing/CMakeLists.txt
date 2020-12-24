project(parsing)

add_library(${PROJECT_NAME}
    Parsing.cpp
    Parsing.hpp
    XmlDefaultFileHandler.cpp
    XmlDefaultFileHandler.hpp
    XmlFileLoader.hpp
    XmlFileSaver.hpp
    XmlFileLoaderMissingRoot.cpp
    XmlFileLoaderMissingRoot.hpp
    XmlDocVersion.hpp
    XmlDocVersion.cpp
)

target_link_libraries(${PROJECT_NAME}
    common
    fs
    logger
)
