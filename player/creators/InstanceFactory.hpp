#pragma once

#include <memory>

template<typename Instance, typename InstanceAdaptor>
class InstanceFactory
{
public:
    template<typename... Args>
    std::unique_ptr<Instance> create(Args&&... args)
    {
        return std::make_unique<Instance>(std::forward<Args>(args)..., std::make_unique<InstanceAdaptor>());
    }

};
