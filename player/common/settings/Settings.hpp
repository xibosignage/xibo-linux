#pragma once

#include "../Field.hpp"
#include "../fs/FilePath.hpp"
#include "../fs/FileSystem.hpp"

#include <boost/property_tree/xml_parser.hpp>

class Settings
{
public:
    virtual void loadFrom(const FilePath& file) = 0;
    virtual void saveTo(const FilePath& file) = 0;

protected:
    ~Settings() = default;

    template<typename... Args>
    void loadFromImpl(const FilePath& file, Field<Args>&... fields)
    {
        if(!FileSystem::exists(file)) return;

        boost::property_tree::ptree tree;
        boost::property_tree::xml_parser::read_xml(file.string(), tree);

        (fields.setValue(tree.get<Args>(std::string{fields.name()})), ...);
    }

    template<typename... Args>
    void saveToImpl(const FilePath& file, Field<Args>... fields)
    {
        boost::property_tree::ptree tree;
        (tree.put(std::string{fields.name()}, fields.value()), ...);

        boost::property_tree::xml_parser::write_xml(file.string(), tree);
    }
};
