#pragma once

#include "common/NamedField.hpp"
#include "common/parsing/Parsing.hpp"

class FilePath;

class PlayerSettings
{
    friend class PlayerSettingsSerializer;

public:
    struct SizeField : NamedField<int, int>
    {
        using NamedField<int, int>::NamedField;

        int width() const
        {
            return value<0>();
        }

        int height() const
        {
            return value<1>();
        }
    };

    struct PositionField : NamedField<int, int>
    {
        using NamedField<int, int>::NamedField;

        int x() const
        {
            return value<0>();
        }

        int y() const
        {
            return value<1>();
        }
    };

public:
    void fromFields(const PlayerSettings& settings);
    void fromFile(const FilePath& file);
    void saveTo(const FilePath& file);

    Field<int>& collectInterval();
    const Field<int>& collectInterval() const;

    Field<bool>& statsEnabled();
    const Field<bool>& statsEnabled() const;

    Field<std::string>& xmrNetworkAddress();
    const Field<std::string>& xmrNetworkAddress() const;

    Field<std::string>& logLevel();
    const Field<std::string>& logLevel() const;

    Field<int>& screenshotInterval();
    const Field<int>& screenshotInterval() const;

    Field<unsigned short>& embeddedServerPort();
    const Field<unsigned short>& embeddedServerPort() const;

    Field<bool>& preventSleep();
    const Field<bool>& preventSleep() const;

    Field<std::string>& displayName();
    const Field<std::string>& displayName() const;

    SizeField& size();
    const SizeField& size() const;

    PositionField& position();
    const PositionField& position() const;

private:
    NamedField<int> collectInterval_{"collectInterval", 900};
    NamedField<bool> statsEnabled_{"statsEnabled", false};  // FIXME should listen to value
    NamedField<std::string> xmrNetworkAddress_{"xmrNetworkAddress"};
    NamedField<std::string> logLevel_{"logLevel", "debug"};
    NamedField<int> screenshotInterval_{"screenshotInterval", 0};
    NamedField<unsigned short> embeddedServerPort_{"embeddedServerPort",
                                                   9696};   // FIXME should listen to value changed and do reconfig
    NamedField<bool> preventSleep_{"preventSleep", false};  // FIXME should listen to value changed and do reconfig
    NamedField<std::string> displayName_{"displayName", "Display"};    // FIXME should listen to value
    SizeField size_{{"sizeX", 0}, {"sizeY", 0}};
    PositionField position_{{"offsetX", 0}, {"offfsetY", 0}};
};
