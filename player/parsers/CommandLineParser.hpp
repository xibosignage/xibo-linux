#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

class CommandLineParser
{
public:
    CommandLineParser();
    void parse(int argc, char** argv);

    bool version_option() const;
    bool help_option() const;
    bool log_level_option() const;
    int log_level() const;
    bool host_option() const;
    std::string host() const;
    bool server_key_option() const;
    std::string server_key() const;
    bool hardware_key_option() const;
    std::string hardware_key() const;

    bool credentials() const;

    const boost::program_options::options_description& available_options() const;

private:
    boost::program_options::variables_map m_vm;
    boost::program_options::options_description m_options;

};
