project(config)

add_library(${PROJECT_NAME}
    PlayerSettings.cpp
    PlayerSettings.hpp
    PlayerSettingsSerializer.cpp
    PlayerSettingsSerializer.hpp
    CmsSettings.cpp
    CmsSettings.hpp
    CmsSettingsSerializer.cpp
    CmsSettingsSerializer.hpp
    SettingsSerializer.hpp
    AppConfig.cpp
    AppConfig.hpp
)
target_link_libraries(${PROJECT_NAME}
    common
)
