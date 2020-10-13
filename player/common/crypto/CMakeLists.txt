project(crypto)

find_library(CRYPTOPP NAMES cryptopp) # TODO find include directories

pkg_check_modules(OPENSSL REQUIRED openssl)

add_library(${PROJECT_NAME}
    CryptoUtils.cpp
    CryptoUtils.hpp
    RsaManager.cpp
    RsaManager.hpp
    Md5Hash.cpp
    Md5Hash.hpp
)

target_link_libraries(${PROJECT_NAME}
    config
    fs
    logger
    Boost::boost
    ${CRYPTOPP}
    ${OPENSSL_LINK_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${OPENSSL_INCLUDE_DIRS}
)
