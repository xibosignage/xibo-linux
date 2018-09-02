#include "CommandLineParser.hpp"
#include "constants.hpp"

#include <boost/program_options/parsers.hpp>

namespace po = boost::program_options;

static const char* VERSION = "version";
static const char* HELP = "help";
static const char* HOST = "host";
static const char* SERVER_KEY = "server-key";
static const char* HARDWARE_KEY = "hardware-key";

CommandLineParser::CommandLineParser() :
    m_options("Allowed options")
{
}

void CommandLineParser::parse(int argc, char** argv)
{
    m_options.add_options()(VERSION, "get project version");
    m_options.add_options()(HELP, "get available options");
    m_options.add_options()(HOST, po::value<std::string>(), "set CMS host");
    m_options.add_options()(SERVER_KEY, po::value<std::string>(), "set server key that used in CMS");
    m_options.add_options()(HARDWARE_KEY, po::value<std::string>(), "set hardware key that identifies your PC");

    po::store(po::parse_command_line(argc, argv, m_options), m_vm);
    po::notify(m_vm);
}

bool CommandLineParser::versionOption() const
{
    return m_vm.count(VERSION);
}

bool CommandLineParser::helpOption() const
{
    return m_vm.empty() || m_vm.count(HELP);
}

bool CommandLineParser::hostOption() const
{
    return m_vm.count(HOST);
}

std::string CommandLineParser::host() const
{
    if(!hostOption())
        throw std::runtime_error("Host option was not parsed");

    return m_vm[HOST].as<std::string>();
}

bool CommandLineParser::serverKeyOption() const
{
    return m_vm.count(SERVER_KEY);
}

std::string CommandLineParser::serverKey() const
{
    if(!serverKeyOption())
        throw std::runtime_error("Server Key option was not parsed");

    return m_vm[SERVER_KEY].as<std::string>();
}

bool CommandLineParser::hardwareKeyOption() const
{
    return m_vm.count(HARDWARE_KEY);
}

std::string CommandLineParser::hardwareKey() const
{
    if(!hardwareKeyOption())
        throw std::runtime_error("Hardware Key option was not parsed");

    return m_vm[HARDWARE_KEY].as<std::string>();
}

bool CommandLineParser::credentials() const
{
    return hostOption() && serverKeyOption() && hardwareKeyOption();
}

const po::options_description& CommandLineParser::availableOptions() const
{
    return m_options;
}
