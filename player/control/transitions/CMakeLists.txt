project(transitions)

add_library(${PROJECT_NAME}
    FadeTransitionExecutor.cpp
    FadeTransitionExecutor.hpp
    FlyTransitionExecutor.cpp
    FlyTransitionExecutor.hpp
    Transition.cpp
    Transition.hpp
    TransitionExecutor.cpp
    TransitionExecutor.hpp
)

target_link_libraries(${PROJECT_NAME}
    widgets
)
