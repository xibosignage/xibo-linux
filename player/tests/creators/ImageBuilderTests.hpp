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
    }

    ImageBuilderTest& adaptor(std::unique_ptr<testing::NiceMock<MockImageAdaptor>> adaptor)
    {
        m_adaptor = std::move(adaptor);
        return *this;
    }

protected:
    std::unique_ptr<IImageAdaptor> createHandler() override
    {
        return std::move(m_adaptor);
    }

private:
    std::unique_ptr<testing::NiceMock<MockImageAdaptor>> m_adaptor;

};
