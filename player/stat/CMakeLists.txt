project(stat)

add_library(${PROJECT_NAME}
    PlayingStat.hpp
    StatsFormatter.cpp
    StatsFormatter.hpp
    StatsRecorder.cpp
    StatsRecorder.hpp
)
