#pragma once

#include "XmdsCommand.hpp"

template <typename SubmitCall>
class SubmitXmdsCommand : public XmdsCommand<SubmitCall>
{
    using SignalSuccess = boost::signals2::signal<void()>;
    using SignalFailed = boost::signals2::signal<void()>;

    using Response = typename SubmitCall::Response;

public:
    using XmdsCommand<SubmitCall>::XmdsCommand;

    SignalSuccess& success()
    {
        return success_;
    }

    SignalFailed& failed()
    {
        return failed_;
    }

protected:
    void process(const Response& response) override
    {
        if (response.success)
        {
            success_();
        }
        else
        {
            failed_();
        }
    }

private:
    SignalSuccess success_;
    SignalFailed failed_;
};
