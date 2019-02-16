//#pragma once

//#include "test_utils.hpp"
//#include "creators/RegionBuilder.hpp"

//#include "control/IRegion.hpp"
//#include "mocks/MockFixedLayoutAdaptor.hpp"
//#include "mocks/MockRegionContent.hpp"
//#include "mocks/MockWidgetAdaptor.hpp"

//#include <boost/optional/optional.hpp>

//const int MIN_WIDTH = 1;
//const int MIN_HEIGHT = 1;
//const auto invalidRegionSizes = invalidSizes<MIN_WIDTH, MIN_HEIGHT>;

//class RegionBuilderSizeTest : public testing::TestWithParam<Size> { };

//class RegionBuilderTest : public RegionBuilder
//{
//public:
//    RegionBuilderTest& adaptor(std::unique_ptr<testing::NiceMock<MockFixedLayoutAdaptor>>&& adaptor)
//    {
//        m_adaptor = std::move(adaptor);
//        return static_cast<RegionBuilderTest&>(*this);
//    }

//    RegionBuilderTest& defaultContent()
//    {
//        std::vector<ContentWithPos> allContent;
//        allContent.push_back(ContentWithPos{constructMock<MockRegionContent, MockWidgetAdaptor>(), DEFAULT_XPOS, DEFAULT_YPOS});
//        return static_cast<RegionBuilderTest&>(content(std::move(allContent)));
//    }

//    RegionBuilder& defaultSize()
//    {
//        return width(DEFAULT_WIDTH).height(DEFAULT_HEIGHT);
//    }

//protected:
//    std::unique_ptr<IFixedLayoutAdaptor> createAdaptor() override
//    {
//        if(m_adaptor)
//            return std::move(m_adaptor);

//        return std::make_unique<testing::NiceMock<MockFixedLayoutAdaptor>>();
//    }

//private:
//    std::unique_ptr<testing::NiceMock<MockFixedLayoutAdaptor>> m_adaptor;

//};
