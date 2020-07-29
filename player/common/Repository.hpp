#pragma once

#include <boost/optional/optional.hpp>
#include <map>
#include <memory>

template <typename StoredType, typename Key = std::string>
class Repository
{
public:
    boost::optional<StoredType&> get(const Key& key) const
    {
        if (isInRepo(key))
        {
            return *m_objects.at(key);
        }

        return {};
    }

    void add(const Key& key, std::unique_ptr<StoredType>&& object)
    {
        m_objects.emplace(key, std::move(object));
    }

private:
    bool isInRepo(const Key& key) const
    {
        return m_objects.count(key) > 0;
    }

private:
    std::map<Key, std::unique_ptr<StoredType>> m_objects;
};
