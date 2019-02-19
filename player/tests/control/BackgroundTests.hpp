#pragma once

#include "BaseTestWithHandler.hpp"

#include "control/Background.hpp"
#include "creators/BackgroundBuilder.hpp"

#include "mocks/MockImageAdaptor.hpp"
#include "mocks/MockFileSystemAdaptor.hpp"

struct Color
{
    std::string strColor;
    uint numberColor;
};

const std::string DEFAULT_COLOR = "#fff";

template<typename T>
class BackgroundTest : public BaseTestWithHandler<MockImageAdaptor>
{
public:
    BackgroundTest()
    {
    }

    auto constructBackground(int width, int height, boost::optional<std::string> path, boost::optional<std::string> color)
    {
        BackgroundBuilder builder;
        builder.createAdaptor(unique(&this->adaptor())).filesystem(unique(&this->filesystem()));

        if constexpr(std::is_same_v<T, OneColorBackground>)
        {
            ResourcesXlf::BackgroundOptions opts{width, height, {}, color.value()};
            return builder.options(opts).build();
        }
        else
        {
            ResourcesXlf::BackgroundOptions opts{width, height, path.value(), {}};
            return builder.options(opts).build();
        }
    }

    auto constructBackground()
    {
        return constructBackground(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH.string(), DEFAULT_COLOR);
    }

protected:
    void doSetUp() override
    {
        m_fakeFilesystem = new testing::NiceMock<MockFileSystemAdaptor>();

        ON_CALL(filesystem(), isRegularFile(testing::_)).WillByDefault(testing::Return(true));
    }

    MockFileSystemAdaptor& filesystem()
    {
        return *m_fakeFilesystem;
    }

private:
    MockFileSystemAdaptor* m_fakeFilesystem;

};

class ImageBackgroundTest : public BackgroundTest<ImageBackground> { };
class OneColorBackgroundTest : public BackgroundTest<OneColorBackground> { };

class ImageBackgroundSizeTest : public ImageBackgroundTest, public testing::WithParamInterface<Size> { };
class OneColorBackgroundSizeTest : public OneColorBackgroundTest, public testing::WithParamInterface<Size> { };
class OneColorBackgroundValidColorTest : public OneColorBackgroundTest, public testing::WithParamInterface<Color> { };
class OneColorBackgroundInvalidColorTest : public OneColorBackgroundTest, public testing::WithParamInterface<std::string> { };
