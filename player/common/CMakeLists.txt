project(common)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
find_library(CryptoPP NAMES cryptopp PATHS "/usr/local/lib" NO_DEFAULT_PATH)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp
                  ${CMAKE_CURRENT_SOURCE_DIR}/crypto/*.*pp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dt/*.*pp
                  ${CMAKE_CURRENT_SOURCE_DIR}/fs/*.*pp
                  ${CMAKE_CURRENT_SOURCE_DIR}/logger/*.*pp
                  ${CMAKE_CURRENT_SOURCE_DIR}/settings/*.*pp
                  ${CMAKE_CURRENT_SOURCE_DIR}/uri/*.*pp
                  ${CMAKE_CURRENT_SOURCE_DIR}/types/*.*pp)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ssl dbus-1 crypto ${CryptoPP} ${Glibmm_LINK_LIBRARIES} ${Boost_LIBRARIES})

add_subdirectory(tests)
