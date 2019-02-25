#pragma once

#include "constants.hpp"
#include "utils/Helpers.hpp"
#include "utils/FilePath.hpp"
#include "utils/Uri.hpp"

const std::string DEFAULT_RESOURCES_DIR = "fakedir";
const FilePath DEFAULT_PATH = "fakepath";
const Uri DEFAULT_URI = Uri{Uri::Scheme::File, DEFAULT_RESOURCES_DIR / DEFAULT_PATH};
const int DEFAULT_DURATION = 10;
const int DEFAULT_ID = 0;

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

const double DEFAULT_XSCALE = 0.5;
const double DEFAULT_YSCALE = 0.5;

const int NEW_DEFAULT_WIDTH = 200;
const int NEW_DEFAULT_HEIGHT = 200;
