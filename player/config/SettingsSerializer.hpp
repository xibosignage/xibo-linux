#pragma once

#include "common/Field.hpp"
#include "common/Parsing.hpp"
#include "common/PlayerRuntimeError.hpp"
#include "common/fs/FilePath.hpp"
#include "common/fs/FileSystem.hpp"
#include "constants.hpp"

template <typename Settings>
class SettingsSerializer
{
public:
    struct Error : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

    virtual void loadFrom(const FilePath& file, Settings& settings) = 0;
    virtual void saveTo(const FilePath& file, const Settings& settings) = 0;

protected:
    template <typename... Args>
    void loadFromImpl(const FilePath& file, Field<Args>&... fields)
    {
        using namespace std::string_literals;

        if (!FileSystem::exists(file)) return;
        try
        {
            auto tree = Parsing::xmlFrom(file);

            (fields.setValue(tree.get<Args>(std::string{fields.name()}, fields.value())), ...);
        }
        catch (std::exception& e)
        {
            throw SettingsSerializer::Error{"Settings", "Load settings error: "s + e.what()};
        }
    }

    template <typename... Args>
    void saveToImpl(const FilePath& file, Field<Args>... fields)
    {
        using namespace std::string_literals;

        try
        {
            XmlNode tree;
            (tree.put(std::string{fields.name()}, fields.value()), ...);

            Parsing::xmlTreeToFile(file, tree);
        }
        catch (std::exception& e)
        {
            throw SettingsSerializer::Error{"Settings", "Save settings error: "s + e.what()};
        }
    }
};
