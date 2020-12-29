#pragma once

#include "common/NamedField.hpp"
#include "common/PlayerRuntimeError.hpp"
#include "common/parsing/XmlDefaultFileHandler.hpp"
#include "common/parsing/XmlFileLoaderMissingRoot.hpp"

template <typename Settings>
class SettingsSerializer : public XmlDefaultFileHandler
{
    static inline const NodePath RootNode{"settings"};
    static inline const NodePath VersionAttr = Parsing::xmlAttr("version");

public:
    struct Error : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

    virtual void loadSettingsFrom(const FilePath& file, Settings& settings) = 0;
    virtual void saveSettingsTo(const FilePath& file, const Settings& settings) = 0;

protected:
    template <typename... Args>
    void loadFromImpl(const XmlNode& tree, Args&... fields)
    {
        using namespace std::string_literals;

        try
        {
            auto root = tree.get_child(RootNode);
            (loadField(root, fields, std::make_index_sequence<fields.size()>{}), ...);
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
    XmlNode saveToImpl(const Args&... fields)
    {
        XmlNode tree;

        auto& root = tree.add_child(RootNode, {});
        (saveField(root, fields, std::make_index_sequence<fields.size()>{}), ...);

        return tree;
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

    NodePath versionAttributePath() const override
    {
        return RootNode / VersionAttr;
    }

    std::unique_ptr<XmlFileLoader> backwardCompatibleLoader(const XmlDocVersion& version) const override
    {
        if (version == XmlDocVersion{"1"}) return std::make_unique<XmlFileLoaderMissingRoot>(RootNode);
        return nullptr;
    }
};
