project(types)

add_library(${PROJECT_NAME}
    internal/ColorToHexConverter.cpp
    internal/ColorToHexConverter.hpp
    internal/StrongType.hpp
    internal/WeakType.hpp
    internal/UriParser.cpp
    internal/UriParser.hpp
    internal/Authority.cpp
    internal/Host.cpp
    internal/Port.cpp
    internal/Scheme.cpp
    internal/UserInfo.cpp
    Color.hpp
    Uri.cpp
    Uri.hpp
)

target_link_libraries(${PROJECT_NAME}
    fs
    Boost::boost
)

add_subdirectory(tests)
