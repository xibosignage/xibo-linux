project(logger)

find_package(spdlog 1.4.1 REQUIRED)

add_library(${PROJECT_NAME}
    Logging.cpp
    Logging.hpp
    XmlLoggerSink.hpp
    XmlLogsRepo.cpp
    XmlLogsRepo.hpp
    XmlLogsRetriever.cpp
    XmlLogsRetriever.hpp
)

target_link_libraries(${PROJECT_NAME}
    dt
    spdlog::spdlog
)
