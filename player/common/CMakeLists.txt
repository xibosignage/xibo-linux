project(common)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
find_library(CryptoPP NAMES cryptopp PATHS "/usr/local/lib" NO_DEFAULT_PATH)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} networking ${CryptoPP} ${Glibmm_LINK_LIBRARIES})
