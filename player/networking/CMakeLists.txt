project(networking)

add_library(${PROJECT_NAME}
    HttpClient.cpp
    HttpClient.hpp
    HttpSession.cpp
    HttpSession.hpp
    HttpRequest.hpp
    ProxyHttpRequest.hpp
    ResponseResult.hpp
)

target_link_libraries(${PROJECT_NAME}
    common
)
