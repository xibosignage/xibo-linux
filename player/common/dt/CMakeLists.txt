project(dt)

add_library(${PROJECT_NAME}
    DateTime.cpp
    DateTime.hpp
    Timer.cpp
    Timer.hpp
)

target_link_libraries(${PROJECT_NAME}
    Boost::date_time
    ${GLIBMM_LINK_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${GLIBMM_INCLUDE_DIRS} # TODO remove with workaround
)
