#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>

using ptree_node = boost::property_tree::ptree;

const std::string SpdLogger = "logger";
const char* const XmrChannel = "playerLinux";
const char* const HeartbeatChannel = "H";

const int MaxVolume = 100;
const int MinVolume = 0;

const int MinDisplayWidth = 160;
const int MaxDisplayWidth = 9999;
const int MinDisplayHeight = 120;
const int MaxDisplayHeight = 9999;

const int DefaultXPos = 0;
const int DefaultYPos = 0;
const int InvalidPos = -1;

const int DefaultHttpVersion = 11;
