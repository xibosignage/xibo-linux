project(Test)

find_package(SqliteOrm REQUIRED)

add_executable(Test main.cpp)

target_link_libraries(Test PRIVATE stat dt sqlite_orm::sqlite_orm)
