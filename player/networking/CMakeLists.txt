project(networking)

add_library(${PROJECT_NAME}
    HttpClient.cpp
    HttpClient.hpp
    HttpSession.cpp
    HttpSession.hpp
    HttpRequest.hpp
    ProxyHttpRequest.hpp
    RequiredItems.cpp
    RequiredItems.hpp
    ResponseResult.hpp
)
target_link_libraries(${PROJECT_NAME}
    common
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
