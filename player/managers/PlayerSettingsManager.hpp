#pragma once

#include "utils/Field.hpp"
#include "utils/FilePath.hpp"
#include "utils/IFileSystemAdaptor.hpp"
#include "model/PlayerSettings.hpp"

#include <boost/property_tree/xml_parser.hpp>

class FilePath;

class PlayerSettingsManager
{
public:
    PlayerSettingsManager(const FilePath& settingsFile);
    void load();
    void update(const Settings& settings);
    void updateCmsSettings(const CmsSettings& settings);
    void updatePlayerSettings(const PlayerSettings& settings);

    PlayerSettings playerSettings();
    CmsSettings cmsSettings();

private:
    template<typename... Args>
    void loadHelper(Field<Args>&... fields)
    {
        if(!m_filesystem->exists(m_settingsFile)) return;

        boost::property_tree::ptree tree;
        boost::property_tree::xml_parser::read_xml(m_settingsFile.string(), tree);

        (fields.setValue(tree.get<Args>(std::string{fields.name()})), ...);
    }

    template<typename... Args>
    void updateHelper(Field<Args>... fields)
    {
        boost::property_tree::ptree tree;
        (tree.put(std::string{fields.name()}, fields.value()), ...);

        boost::property_tree::xml_parser::write_xml(m_settingsFile.string(), tree);
    }

private:
    Settings m_settings;
    FilePath m_settingsFile;
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;

};
