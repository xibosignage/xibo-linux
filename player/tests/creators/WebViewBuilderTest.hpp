#pragma once

#include "creators/MediaBuilderTest.hpp"
#include "creators/WebViewBuilder.hpp"
#include "mocks/MockWebViewAdaptor.hpp"

class WebViewBuilderTest : public MediaBuilderTest<WebViewBuilder>
{
public:
    WebViewBuilderTest()
    {
        m_adaptor = std::make_unique<testing::NiceMock<MockWebViewAdaptor>>();

        ON_CALL(filesystem(), isRegularFile(testing::_)).WillByDefault(testing::Return(true));
    }

    WebViewBuilderTest& adaptor(std::unique_ptr<testing::NiceMock<MockWebViewAdaptor>> adaptor)
    {
        m_adaptor = std::move(adaptor);
        return *this;
    }

protected:
    std::unique_ptr<IWebViewAdaptor> createHandler() override
    {
        return std::move(m_adaptor);
    }

private:
    std::unique_ptr<testing::NiceMock<MockWebViewAdaptor>> m_adaptor;

};
