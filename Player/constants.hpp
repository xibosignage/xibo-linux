#pragma once

#include <string>
#include <gstreamermmconfig.h>
#include <glib.h>

const std::string LOGGER = "logger";

// FIXME remove from constants.hpp
struct Point
{
    int left;
    int top;
};

// FIXME remove from constants.hpp
struct Size
{
    int width;
    int height;
};

G_BEGIN_DECLS

#define	GSTMM_CHECK_VERSION(major,minor,micro)	\
     (GSTREAMERMM_MAJOR_VERSION > (major) || \
     (GSTREAMERMM_MAJOR_VERSION == (major) && GSTREAMERMM_MINOR_VERSION > (minor)) || \
     (GSTREAMERMM_MAJOR_VERSION == (major) && GSTREAMERMM_MINOR_VERSION == (minor) && \
      GSTREAMERMM_MICRO_VERSION >= (micro)))

G_END_DECLS

