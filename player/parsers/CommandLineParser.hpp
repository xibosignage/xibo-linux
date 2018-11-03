#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

class CommandLineParser
{
public:
    CommandLineParser();
    void parse(int argc, char** argv);

    bool versionOption() const;
    bool helpOption() const;
    bool exampleDir() const;
    std::string getExampleDir() const;
    bool hostOption() const;
    std::string host() const;
    bool serverKeyOption() const;
    std::string serverKey() const;
    bool hardwareKeyOption() const;
    std::string hardwareKey() const;

    bool credentials() const;

    const boost::program_options::options_description& availableOptions() const;

private:
    boost::program_options::variables_map m_vm;
    boost::program_options::options_description m_options;

};
