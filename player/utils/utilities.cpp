#include "utilities.hpp"
#include "constants.hpp"
#include "XiboApp.hpp"

#include <boost/property_tree/xml_parser.hpp>

std::shared_ptr<spdlog::logger> utils::logger()
{
    static auto logger = spdlog::get(LOGGER);
    return logger;
}

DownloadManager& utils::downloadManager()
{
    return XiboApp::app().downloadManager();
}

XMDSManager& utils::xmdsManager()
{
    return XiboApp::app().xmdsManager();
}

std::filesystem::path utils::resourcesDir()
{
    return downloadManager().resourcesDir();
}

boost::property_tree::ptree utils::parseXmlFromPath(const std::filesystem::path& xlfPath)
{
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(xlfPath.string(), tree);
    return tree;
}

boost::property_tree::ptree utils::parseXmlFromString(const std::string& xml)
{
    std::stringstream stream;
    stream << xml;
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(stream, tree);
    return tree;
}
