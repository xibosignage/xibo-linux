#ifndef RESPONSESFWD_HPP
#define RESPONSESFWD_HPP

#include "constants.hpp"

class RegisterDisplayResponse;
class RequiredFilesResponse;
class ScheduleResponse;
class GetFileResponse;

template<const char* ResponseName>
class SuccessResponse;

using NotifyStatusResponse = SuccessResponse<NOTIFY_STATUS>;
using SubmitLogResponse = SuccessResponse<SUBMIT_LOG>;
using SubmitStatsResponse = SuccessResponse<SUBMIT_STATS>;

#endif // RESPONSESFWD_HPP
