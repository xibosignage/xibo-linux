#pragma once

#include "test_utils.hpp"

#include "creators/ImageBuilder.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockMedia.hpp"
#include "mocks/MockImageAdaptor.hpp"

#include "mocks/MockFileSystemAdaptor.hpp"

template<typename T>
class MediaBuilderTest : public T
{
public:
    MediaBuilderTest()
    {
        m_filesystem = std::make_unique<testing::NiceMock<MockFileSystemAdaptor>>();
    }

    T& defaultBaseProps()
    {
        ON_CALL(*m_filesystem, isRegularFile(testing::_)).WillByDefault(testing::Return(true));;

        return static_cast<T&>(this->path(DEFAULT_PATH.string()).duration(DEFAULT_DURATION));
    }

    testing::NiceMock<MockFileSystemAdaptor>& filesystem()
    {
        return *m_filesystem;
    }

private:
    std::unique_ptr<testing::NiceMock<MockFileSystemAdaptor>> m_filesystem;

};

class ImageBuilderTest : public MediaBuilderTest<ImageBuilder>
{
public:
    ImageBuilderTest()
    {
        m_adaptor = std::make_unique<testing::NiceMock<MockImageAdaptor>>();
    }

    testing::NiceMock<MockImageAdaptor>& adaptor()
    {
        return *m_adaptor;
    }

    ImageBuilderTest& defaultSize()
    {
        return static_cast<ImageBuilderTest&>(width(DEFAULT_WIDTH).height(DEFAULT_HEIGHT));
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
