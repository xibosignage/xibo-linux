#pragma once

#include "BaseLayoutBuilder.hpp"

class MainLayoutBuilder : public BaseLayoutBuilder
{
protected:
    std::shared_ptr<IOverlayLayout> createView(const LayoutOptions& options) override;

private:
    std::shared_ptr<IImage> createBackground(const LayoutOptions& options);

};
