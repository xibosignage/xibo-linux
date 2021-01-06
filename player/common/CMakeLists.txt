project(common)

find_package(Boost 1.70 EXACT REQUIRED system thread filesystem date_time)

# TODO move to utils
add_library(${PROJECT_NAME}
    constants.hpp
    Field.hpp
    JoinableThread.hpp
    NamedField.hpp
    PlayerError.cpp
    PlayerError.hpp
    PlayerRuntimeError.hpp
    Repository.hpp
    Singleton.hpp
    SoapField.hpp
    Utils.hpp
)

target_link_libraries(${PROJECT_NAME}
    Boost::thread
    Boost::system
)

add_subdirectory(fs)
add_subdirectory(dt)
add_subdirectory(parsing)
add_subdirectory(crypto)
add_subdirectory(logger)
add_subdirectory(system)
add_subdirectory(types)
add_subdirectory(storage)
