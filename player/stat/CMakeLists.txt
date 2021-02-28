project(stat)

find_package(SqliteOrm REQUIRED)

add_library(${PROJECT_NAME}
    PlayingTime.hpp
    XmlFormatter.cpp
    XmlFormatter.hpp
    Recorder.cpp
    Recorder.hpp
    DataProvider.hpp
    PlayingRecord.hpp
    PlayingRecord.cpp
    PlayingRecords.hpp
    Records.hpp
    PlayingRecordType.cpp
    PlayingRecordType.hpp
    IRecordVisitor.hpp
    DtoConverter.cpp
    DtoConverter.hpp
    DatabaseProvider.cpp
    DatabaseProvider.hpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE dt sqlite_orm::sqlite_orm)
