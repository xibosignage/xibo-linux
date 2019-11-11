project(networking)

file(GLOB XMDS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/xmds/*.*pp
)
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
    WebServer.cpp
    WebServer.hpp
    ${XMDS_SOURCES}
)
target_link_libraries(${PROJECT_NAME}
    common
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
