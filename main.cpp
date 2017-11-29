#include "Layout.hpp"
#include "Image.hpp"
#include "utilities.hpp"
#include "LayoutParser.hpp"

#include <iostream>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

int main()
{
    std::fstream file("20.xlf");

    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(file, tree);


    auto root = tree.get_child("layout");
    auto layout = std::shared_ptr<Layout>(utilities::GetParser<LayoutParser>(root)->Parse());
    std::cout << layout->schemaVersion << " " << layout->width << " " << layout->height << " " << layout->backgroundColor << " " << layout->backgroundImage << std::endl;
    for(auto region : layout->regions)
    {
        std::cout << "region " << region->id << " " << region->width << " " << region->height << " " << region->top << " " << region->left << " " << region->zindex << " " << region->options.loop << std::endl;
        for(auto media : region->medias)
        {
            if(auto image = dynamic_cast<Image*>(media.get()))
                std::cout << (int)image->options.align << " " << (int)image->options.scaleType << " " << (int)image->options.valign << " ";
            std::cout << (int)media->render << " " << media->id << " " << media->duration << " " << media->options.uri << std::endl;
        }
    }

    return 0;
}
