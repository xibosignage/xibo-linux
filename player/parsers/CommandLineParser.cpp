#include "CommandLineParser.hpp"
#include "constants.hpp"

#include <boost/filesystem.hpp>
#include <spdlog/fmt/ostr.h>

namespace fs = boost::filesystem;
namespace po = boost::program_options;

const char* EXAMPLE_DIR = "example-dir";
const char* VERSION = "version";
const char* UNIT_TESTING = "unit-testing";
const char* HELP = "help";
const char* LOG_LEVEL = "log-level";

CommandLineParser::CommandLineParser() :
    m_options("Allowed options")
{
    m_logger = spdlog::get(LOGGER);
}

bool CommandLineParser::parse(int argc, char** argv)
{
    po::variables_map vm;
    m_options.add_options()(EXAMPLE_DIR, po::value<std::string>()->value_name("path-to-dir"), "specify full (absolute) path to example directory");
    m_options.add_options()(VERSION, "get project version");
    m_options.add_options()(UNIT_TESTING, "enable unit testing mode");
    m_options.add_options()(HELP, "get available options");
    m_options.add_options()(LOG_LEVEL, po::value<int>()->value_name("[0-6]"), "set logging level (0 for all logs, 6 turn off)");

    try {
        po::store(po::parse_command_line(argc, argv, m_options), vm);
        po::notify(vm);
    }
    catch(std::exception& ex) {
        m_logger->error(ex.what());
        return false;
    }

    if(vm.empty() || vm.count(HELP)){
        m_logger->info("{}", m_options);
        return false;
    }
    if(vm.count(LOG_LEVEL)) {
        spdlog::set_level(static_cast<spdlog::level::level_enum>(vm[LOG_LEVEL].as<int>()));
    }
    if(vm.count(EXAMPLE_DIR)) {
        auto example_dir = vm[EXAMPLE_DIR].as<std::string>();
        bool dir_exists = check_example_dir(example_dir);
        if(dir_exists)
        {
            auto [xlf_exists, xlf_path] = find_xlf_file(example_dir);
            if(xlf_exists)
            {
                m_example_dir_path = example_dir;
                m_xlf_path = xlf_path;
                m_is_example_dir = true;
            }
        }
    }
    if(vm.count(UNIT_TESTING)) {
        m_is_testing = true;
    }
    if(vm.count(VERSION)) {
        m_is_version = true;
    }
    return true;
}

bool CommandLineParser::check_example_dir(const std::string& example_dir)
{
    if(!fs::exists(example_dir) || !fs::is_directory(example_dir))
    {
        m_logger->error("The directory doesn't exist (or it isn't a directory)");
        return false;
    }
    m_logger->info("Example directory is {}", example_dir);

    return true;
}

bool CommandLineParser::is_version() const
{
    return m_is_version;
}

bool CommandLineParser::is_example_dir() const
{
    return m_is_example_dir;
}

bool CommandLineParser::is_testing() const
{
    return m_is_testing;
}

const std::string& CommandLineParser::xlf_path() const
{
    return m_xlf_path;
}

const std::string& CommandLineParser::example_dir_path() const
{
    return m_example_dir_path;
}

const po::options_description& CommandLineParser::available_options() const
{
    return m_options;
}

std::pair<bool, std::string> CommandLineParser::find_xlf_file(const std::string& example_dir_path)
{
    fs::directory_iterator it(example_dir_path);
    fs::directory_iterator end;

    while(it != end)
    {
        if(fs::is_regular_file(*it) && it->path().extension() == ".xlf")
            return {true, it->path().string()};
        ++it;
    }

    return {false, std::string{}};
}
