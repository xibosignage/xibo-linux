#pragma once

#include "test_utils.hpp"
#include "BaseTestWithHandler.hpp"

#include "mocks/MockFileSystem.hpp"

template<typename T>
class MediaTest : public BaseTestWithHandler<T>
{
protected:
    MockFileSystemAdaptor& filesystem()
    {
        return *m_filesystem;
    }

    void doSetUp() override
    {
        m_filesystem = new testing::NiceMock<MockFileSystemAdaptor>();

        ON_CALL(filesystem(), isRegularFile(testing::_)).WillByDefault(testing::Return(true));
    }

private:
    MockFileSystemAdaptor* m_filesystem = nullptr;


};
