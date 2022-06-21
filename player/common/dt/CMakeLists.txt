project(dt)

add_library(${PROJECT_NAME}
    DateTime.cpp
    DateTime.hpp
    Timer.cpp
    Timer.hpp
)

find_library(DATE_TZ_LINK_LIBRARY NAMES date-tz)
find_path(DATE_TZ_INCLUDE_DIR NAMES date)

target_link_libraries(${PROJECT_NAME}
    Boost::date_time
    ${GLIBMM_LINK_LIBRARIES}
    ${DATE_TZ_LINK_LIBRARY}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${GLIBMM_INCLUDE_DIRS} # TODO remove with workaround
    PRIVATE ${DATE_TZ_INCLUDE_DIR}
)
