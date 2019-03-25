#pragma once

#include "utils/Field.hpp"
#include "utils/FilePath.hpp"
#include "utils/IFileSystemAdaptor.hpp"

#include <boost/property_tree/xml_parser.hpp>

struct PlayerSettings;
class FilePath;

class PlayerSettingsManager
{
public:
    PlayerSettingsManager(const FilePath& settingsFile);
    PlayerSettings load();
    void update(const PlayerSettings& settings);

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
    FilePath m_settingsFile;
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;

};
