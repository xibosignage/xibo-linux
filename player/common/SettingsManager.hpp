#pragma once

#include "Field.hpp"
#include "FilePath.hpp"
#include "FileSystem.hpp"

#include <boost/property_tree/xml_parser.hpp>

template<typename Settings>
class SettingsManager
{
public:
    SettingsManager(const FilePath& settingsFile) :
        m_settingsFile(settingsFile)
    {
    }

    virtual ~SettingsManager() = default;

    Settings load()
    {
        auto settings = loadImpl();
        m_settings = std::move(settings);
        return m_settings;
    }

    void update(const Settings& settings)
    {
        updateImpl(settings);

        m_settings = settings;
    }

    Settings settings()
    {
        return m_settings;
    }

protected:
    virtual Settings loadImpl() = 0;
    virtual void updateImpl(const Settings& settings) = 0;

    template<typename... Args>
    void loadHelper(Field<Args>&... fields)
    {
        if(!FileSystem::exists(m_settingsFile)) return;

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
    Settings m_settings;

};
