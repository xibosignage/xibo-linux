#pragma once

#include "schedule/LayoutSchedule.hpp"

const int DefaultTestId = 1;
const int DefaultTestPriority = 0;

const std::string ScheduleXml = R"(<?xml version="1.0"?>
                               <schedule generated="2019-01-08 08:00:00" fitlerFrom="2019-07-26 07:00:00" fitlerTo="2019-07-28 07:00:00">
                                 <layout file="3" fromdt="1970-01-01 01:00:00" todt="2038-01-19 03:14:07" scheduleid="2" priority="1" syncEvent="0">
                                   <dependents>
                                     <file>local1.txt</file>
                                     <file>local2.txt</file>
                                   </dependents>
                                 </layout>
                                 <overlays>
                                   <overlay file="2" fromdt="1970-01-01 01:00:00" todt="2038-01-19 03:14:07" scheduleid="1" priority="0"/>
                                 </overlays>
                                 <default file="1">
                                   <dependents>
                                     <file>defaultLocal1.txt</file>
                                     <file>defaultLocal2.txt</file>
                                   </dependents>
                                 </default>
                                 <dependants>
                                   <file>global1.txt</file>
                                   <file>global2.txt</file>
                                 </dependants>
                               </schedule>)";

namespace ScheduleTests
{
    ScheduledLayout scheduledLayout(int scheduledId, int id, int priority);
    ScheduledLayout scheduledLayout();
    LayoutSchedule schedule();
}
