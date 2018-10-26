#pragma once
#include "creators/MediaBuilderTest.hpp"
#include "creators/WebViewBuilder.hpp"
#include "media/MediaVisitor.hpp"
#include "mocks/MockWebViewAdaptor.hpp"

const int DEFAULT_ID = 1;

class WebViewBuilderTest : public MediaBuilderTest<WebViewBuilder>
{
public:
    WebViewBuilderTest()
    {
        m_adaptor = std::make_unique<testing::NiceMock<MockWebViewAdaptor>>();

        ON_CALL(filesystem(), isRegularFile(testing::_)).WillByDefault(testing::Return(true));
        width(DEFAULT_WIDTH).height(DEFAULT_HEIGHT).id(DEFAULT_ID).path(DEFAULT_PATH.string()).duration(DEFAULT_DURATION);
    }

    testing::NiceMock<MockWebViewAdaptor>& adaptor()
    {
        return *m_adaptor;
    }

protected:
    std::unique_ptr<IWebViewAdaptor> createAdaptor() override
    {
        return std::move(m_adaptor);
    }

private:
    std::unique_ptr<testing::NiceMock<MockWebViewAdaptor>> m_adaptor;

};
