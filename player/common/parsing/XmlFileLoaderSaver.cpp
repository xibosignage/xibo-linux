#include "XmlFileLoaderSaver.hpp"

#include "common/fs/FilePath.hpp"
#include "common/logger/Logging.hpp"

const std::string DefaultVersion{"1"};

XmlNode XmlDefaultFileLoader::loadXmlFrom(const FilePath& file)
{
    auto tree = parseXml(file);
    auto version = documentVersion(tree).value_or(DefaultVersion);
    if (version != currentVersion())
    {
        auto filename = file.filename().string();
        Log::debug("[XmlLoader] Trying to use backward compatible loader for {}", filename);

        auto loader = backwardCompatibleLoader(version);
        if (loader)
        {
            return loader->loadXmlFrom(file);
        }

        throw Error{"Can't find loader for " + filename + ": no backward compatible loaders found"};
    }
    return tree;
}

void XmlDefaultFileLoader::saveXmlTo(const FilePath& file, const XmlNode& tree)
{
    try
    {
        Parsing::xmlTreeToFile(file, tree);
    }
    catch (std::exception& e)
    {
        throw Error{e.what()};
    }
}

XmlNode XmlDefaultFileLoader::parseXml(const FilePath& file)
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
