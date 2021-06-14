project(commands)

find_package(SqliteOrm REQUIRED)

add_library(${PROJECT_NAME}
    CommandImpl.cpp
    CommandImpl.hpp
    Command.hpp
    CommandCode.hpp
    CommandsExecutor.hpp
    CommandsExecutor.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE logger)

add_subdirectory(tests)