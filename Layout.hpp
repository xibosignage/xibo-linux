#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include "XiboObject.hpp"
#include "Region.hpp"

#include <vector>
#include <memory>

class Layout : public XiboObject
{
public:
    void InitOptions(std::map<std::string, std::string>) override { }

    int schemaVersion;
    int width;
    int height;
    std::string backgroundImage;
    std::string backgroundColor;

    std::vector<std::shared_ptr<Region>> regions;
};

#endif // LAYOUT_HPP
