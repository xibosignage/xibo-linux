#pragma once

#include "constants.hpp"

#include "model/MainLayoutModel.hpp"
#include "utils/FilePath.hpp"

class MainParser
{
public:
    std::shared_ptr<MainLayoutModel> parseLayoutModel(int layoutId);

private:
    FilePath getXlfFilePath(int layoutId);

    std::shared_ptr<MainLayoutModel> parseLayout(const xml_node& node);
    std::shared_ptr<RegionModel> parseRegion(const xml_node& node);
    std::shared_ptr<MediaModel> parseMedia(const xml_node& node);
    std::shared_ptr<MediaModel> parseImage(const xml_node& node);
    std::shared_ptr<MediaModel> parseAudio(const xml_node& node);
    std::shared_ptr<MediaModel> parseVideo(const xml_node& node);
    std::shared_ptr<MediaModel> parseWebView(const xml_node& node);
    void attachAdditionalMedia(const std::shared_ptr<MediaModel>& media, const xml_node& node);
};
