#pragma once

#include <gmock/gmock.h>

#include "IScheduleSerializer.hpp"

class FakeScheduleSerializer : public IScheduleSerializer
{
public:
    MOCK_METHOD1(parseSchedule, LayoutSchedule(const FilePath& path));
    MOCK_METHOD1(parseSchedule, LayoutSchedule(const std::string& xmlSchedule));
};
