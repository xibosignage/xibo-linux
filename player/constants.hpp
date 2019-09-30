#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>

using PtreeNode = boost::property_tree::ptree;

const std::string SpdLogger = "logger";
const char* const XmrChannel = "playerLinux";
const char* const HeartbeatChannel = "H";

const int MaxVolume = 100;
const int MinVolume = 0;

const int DefaultHttpVersion = 11;
