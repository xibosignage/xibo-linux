#pragma once

#include "test_utils.hpp"

template<typename Handler>
class BaseTestWithHandler : public testing::Test
{
protected:
    virtual void doSetUp() { }
    virtual void doTearDown() { }

    void SetUp() final
    {
        m_adaptor = new testing::NiceMock<Handler>;

        doSetUp();
    }

    void TearDown() final
    {
        doTearDown();
    }

    testing::NiceMock<Handler>& adaptor()
    {
        return *m_adaptor;
    }

private:
    testing::NiceMock<Handler>* m_adaptor = nullptr;
};
