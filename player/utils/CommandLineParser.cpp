#include "CommandLineParser.hpp"
#include "constants.hpp"

#include <boost/filesystem.hpp>
#include <spdlog/fmt/ostr.h>

namespace fs = boost::filesystem;
namespace po = boost::program_options;

CommandLineParser::CommandLineParser() :
    m_options("Allowed options")
{
    m_logger = spdlog::get(LOGGER);
}

bool CommandLineParser::parse(int argc, char** argv)
{
    po::variables_map vm;
    m_options.add_options()("example-dir", po::value<std::string>()->value_name("path-to-dir"), "specify full (absolute) path to example directory");
    m_options.add_options()("version", "get project version");
    m_options.add_options()("testing", "enable testing mode");
    m_options.add_options()("help", "get available options");
    m_options.add_options()("log-level", po::value<int>()->value_name("[0-6]"), "set logging level (0 for all logs, 6 turn off)");

    try {
        po::store(po::parse_command_line(argc, argv, m_options), vm);
        po::notify(vm);
    }
    catch(std::exception& ex) {
        m_logger->error(ex.what());
        return false;
    }

    if(vm.empty() || vm.count("help")){
        m_logger->info("{}", m_options);
        return false;
    }
    if(vm.count("log-level")) {
        spdlog::set_level(static_cast<spdlog::level::level_enum>(vm["log-level"].as<int>()));
    }
    if(vm.count("example-dir")) {
        m_is_example_dir = check_example_dir(vm["example-dir"].as<std::string>());

        if(vm.count("testing")) {
            m_is_testing = true;
        }
    }
    if(vm.count("version")) {
        m_is_version = true;
    }
    return true;
}

bool CommandLineParser::check_example_dir(const std::string& example_dir)
{
    m_example_dir = example_dir;
    if(!fs::exists(m_example_dir) || !fs::is_directory(m_example_dir))
    {
        m_logger->error("The directory doesn't exist (or it isn't a directory)");
        return false;
    }
    m_logger->info("Example directory is {}", m_example_dir);

    m_xlf_file = find_xlf_file(m_example_dir);
    if(m_xlf_file.empty())
    {
        m_logger->error(".XLF file doesn't exist");
        return false;
    }

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

std::string CommandLineParser::xlf_file() const
{
    return m_xlf_file;
}

std::string CommandLineParser::example_dir() const
{
    return m_example_dir;
}

const po::options_description& CommandLineParser::available_options() const
{
    return m_options;
}

std::string CommandLineParser::find_xlf_file(const std::string& example_dir_path)
{
    fs::directory_iterator it(example_dir_path);
    fs::directory_iterator end;

    while(it != end)
    {
        if(fs::is_regular_file(*it) && it->path().extension() == ".xlf")
            return it->path().generic_string();
        ++it;
    }

    return std::string{};
}
