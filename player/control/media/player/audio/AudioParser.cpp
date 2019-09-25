#include "AudioParser.hpp"

#include "control/media/MediaResources.hpp"
#include "control/media/player/MediaPlayerResources.hpp"

#include "control/common/Validators.hpp"

const bool DefaultAudioLooped = false;
const int DefaultDuration = 0;

ExtraOptions AudioParser::extraOptionsImpl(const ptree_node& node)
{
    auto looped = node.get<bool>(XlfResources::Player::Loop, DefaultAudioLooped);
    auto volume = node.get<int>(XlfResources::Player::Volume, MaxVolume);

    return {{XlfResources::Player::Loop, std::to_string(looped)},
            {XlfResources::Player::Volume, std::to_string(volume)}};
}
