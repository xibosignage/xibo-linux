#pragma once

#include "common/NamedField.hpp"
#include "common/Parsing.hpp"
#include "common/PlayerRuntimeError.hpp"
#include "common/fs/FilePath.hpp"
#include "common/fs/FileSystem.hpp"

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
    void loadFromImpl(const FilePath& file, Args&... fields)
    {
        using namespace std::string_literals;

        if (!FileSystem::exists(file)) return;
        try
        {
            auto tree = Parsing::xmlFrom(file);

            (loadField(tree, fields, std::make_index_sequence<fields.size()>{}), ...);
        }
        catch (std::exception& e)
        {
            throw SettingsSerializer::Error{"Settings", "Load settings error: "s + e.what()};
        }
    }

    template <typename... Args, size_t... Is>
    void loadField(const XmlNode& node, NamedField<Args...>& field, std::index_sequence<Is...>)
    {
        if constexpr (field.size() == 1)
        {
            field.setValue(node.get<Args...>(field.name(), field.value()));
        }
        else
        {
            field.setValue(node.get<Args>(field.template name<Is>(), field.template value<Is>())...);
        }
    }

    template <typename... Args>
    void saveToImpl(const FilePath& file, const Args&... fields)
    {
        using namespace std::string_literals;

        try
        {
            XmlNode tree;
            (saveField(tree, fields, std::make_index_sequence<fields.size()>{}), ...);

            Parsing::xmlTreeToFile(file, tree);
        }
        catch (std::exception& e)
        {
            throw SettingsSerializer::Error{"Settings", "Save settings error: "s + e.what()};
        }
    }

    template <typename... Args, size_t... Is>
    void saveField(XmlNode& node, const NamedField<Args...>& field, std::index_sequence<Is...>)
    {
        if constexpr (field.size() == 1)
        {
            node.put(field.name(), field.value());
        }
        else
        {
            (node.put(field.template name<Is>(), field.template value<Is>()), ...);
        }
    }
};
