#include "Layout.hpp"
#include "Image.hpp"
#include "MainParser.hpp"

#include <iostream>

int main()
{
    MainParser parser("20.xlf");

    auto layout = parser.parse();
    std::cout << layout->schema_version() << " " << layout->width() << " " << layout->height() << " " << layout->background_color() << " " << layout->background_image() << std::endl;

    for(auto&& region : layout->regions())
    {
        std::cout << "region " << region->id() << " " << region->width() << " " << region->height() << " " << region->top() << " " << region->left() << " " << region->zindex() << " " << region->loop() << std::endl;
        for(auto&& media : region->medias())
        {
            if(auto image = dynamic_cast<Image*>(media.get()))
                std::cout << (int)image->align() << " " << (int)image->scale_type() << " " << (int)image->valign() << " ";
            std::cout << (int)media->render() << " " << media->id() << " " << media->duration() << " " << media->uri() << std::endl;
        }
    }

    return 0;
}
