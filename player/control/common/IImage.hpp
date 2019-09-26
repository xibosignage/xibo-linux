#pragma once

#include "IWidget.hpp"

#include <cstdint>

class FilePath;

class IImage : public IWidget
{
public:
    virtual void setColor(uint32_t hex_color_number) = 0;
    virtual void loadFromFile(const FilePath& path, bool preserveAspectRatio) = 0;
};
