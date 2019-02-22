#pragma once

#include "constants.hpp"
#include "utils/Helpers.hpp"
#include "utils/FilePath.hpp"

const std::string DEFAULT_RESOURCES_DIR = "fakedir";
const FilePath DEFAULT_PATH = "fakepath";
const FilePath DEFAULT_FULL_PATH = DEFAULT_RESOURCES_DIR / DEFAULT_PATH;
const int DEFAULT_DURATION = 10;
const int DEFAULT_ID = 0;

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

const double DEFAULT_XSCALE = 0.5;
const double DEFAULT_YSCALE = 0.5;

const int NEW_DEFAULT_WIDTH = 200;
const int NEW_DEFAULT_HEIGHT = 200;
