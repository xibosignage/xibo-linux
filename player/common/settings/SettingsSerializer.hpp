#pragma once

#include "../Field.hpp"
#include "../fs/FilePath.hpp"
#include "../fs/FileSystem.hpp"
#include "constants.hpp"

#include <boost/property_tree/xml_parser.hpp>

template <typename Settings>
class SettingsSerializer
{
public:
    virtual void loadFrom(const FilePath& file, Settings& settings) = 0;
    virtual void saveTo(const FilePath& file, const Settings& settings) = 0;

protected:
    ~SettingsSerializer() = default;

    template <typename... Args>
    void loadFromImpl(const FilePath& file, Field<Args>&... fields)
    {
        if (!FileSystem::exists(file)) return;

        boost::property_tree::ptree tree;
        boost::property_tree::xml_parser::read_xml(file.string(), tree);

        (fields.setValue(tree.get<Args>(std::string{fields.name()})), ...);
    }

    template <typename... Args>
    void saveToImpl(const FilePath& file, Field<Args>... fields)
    {
        boost::property_tree::ptree tree;
        (tree.put(std::string{fields.name()}, fields.value()), ...);

        boost::property_tree::xml_parser::write_xml(file.string(), tree);
    }
};
