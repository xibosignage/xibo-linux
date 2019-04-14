#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>

using xml_node = boost::property_tree::ptree;

const std::string LOGGER = "logger";

const unsigned int MSECS = 1000;
const int MAX_VOLUME = 100;
const int MIN_VOLUME = 0;

const int MIN_DISPLAY_WIDTH = 160;
const int MAX_DISPLAY_WIDTH = 9999;
const int MIN_DISPLAY_HEIGHT = 120;
const int MAX_DISPLAY_HEIGHT = 9999;

const int MIN_XPOS = 0;
const int MIN_YPOS = 0;
const int DEFAULT_XPOS = 0;
const int DEFAULT_YPOS = 0;
const int INVALID_POS = -1;

const int DEFAULT_HTTP_VERSION = 11;
const std::string DEFAULT_HTTP_TARGET = "/";
