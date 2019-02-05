#pragma once

#include "creators/MediaBuilderTest.hpp"
#include "creators/ImageBuilder.hpp"
#include "mocks/MockImageAdaptor.hpp"

class ImageBuilderTest : public MediaBuilderTest<ImageBuilder>
{
public:
    ImageBuilderTest()
    {
        m_adaptor = std::make_unique<testing::NiceMock<MockImageAdaptor>>();

        ON_CALL(filesystem(), isRegularFile(testing::_)).WillByDefault(testing::Return(true));
        width(DEFAULT_WIDTH).height(DEFAULT_HEIGHT).path(DEFAULT_PATH.string()).duration(DEFAULT_DURATION);
    }

    testing::NiceMock<MockImageAdaptor>& adaptor()
    {
        return *m_adaptor;
    }

    ImageBuilderTest& defaultImageProps()
    {
        return static_cast<ImageBuilderTest&>(scaleType(DEFAULT_SCALE_TYPE).align(DEFAULT_ALIGN).valign(DEFAULT_VALIGN));
    }

protected:
    std::unique_ptr<IImageAdaptor> createAdaptor() override
    {
        return std::move(m_adaptor);
    }

private:
    std::unique_ptr<testing::NiceMock<MockImageAdaptor>> m_adaptor;

};
