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
    auto layout = utilities::get_parser<LayoutParser>(root)->parse();
    std::cout << layout->schema_version() << " " << layout->width() << " " << layout->height() << " " << layout->background_color() << " " << layout->background_image() << std::endl;
    for(auto region : layout->regions())
    {
        std::cout << "region " << region->id() << " " << region->width() << " " << region->height() << " " << region->top() << " " << region->left() << " " << region->zindex() << " " << region->loop() << std::endl;
        for(auto media : region->medias())
        {
            if(auto image = dynamic_cast<Image*>(media.get()))
                std::cout << (int)image->align() << " " << (int)image->scale_type() << " " << (int)image->valign() << " ";
            std::cout << (int)media->render() << " " << media->id() << " " << media->duration() << " " << media->uri() << std::endl;
        }
    }

    return 0;
}
