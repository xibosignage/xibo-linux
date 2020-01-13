project(common)

find_library(CRYPTOPP NAMES cryptopp) # find include directories
pkg_check_modules(DBUS REQUIRED dbus-1)
pkg_check_modules(OPENSSL REQUIRED openssl)

file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/crypto/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/dt/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/fs/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/system/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/types/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/types/internal/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/types/uri/*.*pp
)

add_library(${PROJECT_NAME}
    ${SOURCES}
)
target_link_libraries(${PROJECT_NAME}
    ${CRYPTOPP}
    ${OPENSSL_LINK_LIBRARIES}
    ${DBUS_LINK_LIBRARIES}
    ${GLIBMM_LINK_LIBRARIES}
    ${Boost_LIBRARIES}
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GLIBMM_INCLUDE_DIRS} # remove this workaround
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${OPENSSL_INCLUDE_DIRS}
    ${DBUS_INCLUDE_DIRS}
)

add_subdirectory(tests)
