#pragma once

#include "constants.hpp"
#include <boost/property_tree/ptree.hpp>

class IMainLayout;

class MainLayoutFactory
{
public:
    MainLayoutFactory(const xlf_node& layoutNode);
    std::unique_ptr<IMainLayout> create();

protected:
    virtual std::unique_ptr<IMainLayout> createLayout(int width, int height,
                                                     const std::string& backgroundImage,
                                                     const std::string& backgroundColor);

private:
    xlf_node m_layoutNode;

};
