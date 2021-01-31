project(system)

pkg_check_modules(DBUS REQUIRED dbus-1)

add_library(${PROJECT_NAME}
    Dbus.cpp
    Dbus.hpp
    HardwareKey.hpp
    HardwareKeyGenerator.cpp
    HardwareKeyGenerator.hpp
    MacAddress.hpp
    MacAddressFetcher.cpp
    MacAddressFetcher.hpp
    Hostname.hpp
    System.cpp
    System.hpp
)

target_link_libraries(${PROJECT_NAME}
    crypto
    Boost::boost
    ${DBUS_LINK_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${DBUS_INCLUDE_DIRS}
)
