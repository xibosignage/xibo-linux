#include "XmlDefaultFileHandler.hpp"

#include "common/fs/FilePath.hpp"
#include "common/logger/Logging.hpp"

const XmlDocVersion DefaultVersion{"1"};

XmlNode XmlDefaultFileHandler::loadXmlFrom(const FilePath& file)
{
    auto tree = parseXml(file);
    auto version = documentVersion(tree);
    if (version != currentVersion())
    {
        auto loader = backwardCompatibleLoader(version);
        if (loader)
        {
            Log::debug("[XmlDefaultFileHandler] Using backward compatible loader (v{}) for {}", version, file);

            auto tree = loader->loadXmlFrom(file);
            saveXmlTo(file, tree);
            return tree;
        }

        throw Error{"Can't find backward compatible loader for " + file.string() + " (v" +
                    static_cast<std::string>(version) + "): data won't be loaded"};
    }
    return tree;
}

void XmlDefaultFileHandler::saveXmlTo(const FilePath& file, const XmlNode& tree)
{
    try
    {
        XmlNode treeWithVersion = tree;

        treeWithVersion.put(versionAttributePath(), currentVersion());

        Parsing::xmlTreeToFile(file, treeWithVersion);
    }
    catch (std::exception& e)
    {
        throw Error{e.what()};
    }
}

XmlDocVersion XmlDefaultFileHandler::documentVersion(const XmlNode& tree) const
{
    return tree.get<XmlDocVersion>(versionAttributePath(), DefaultVersion);
}

XmlNode XmlDefaultFileHandler::parseXml(const FilePath& file)
{
    try
    {
        return Parsing::xmlFrom(file);
    }
    catch (std::exception& e)
    {
        throw Error{e.what()};
    }
}
