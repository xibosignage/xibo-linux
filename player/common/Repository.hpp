#pragma once

#include <memory>
#include <map>
#include <boost/optional/optional.hpp>

template<typename RepoType, typename StoredType, typename Key = std::string>
class Repository
{
protected:
    static boost::optional<StoredType&> get(const Key& key)
    {
        if(instance().isInRepo(key))
        {
            return instance().getObject(key);
        }

        return {};
    }

    static void add(const Key& key, std::unique_ptr<StoredType>&& object)
    {
        instance().addObject(key, std::move(object));
    }

    static RepoType& instance()
    {
        static RepoType repo;
        return repo;
    }

protected:
    bool isInRepo(const Key& key) const
    {
        return m_objects.count(key) > 0;
    }

    const StoredType& getObject(const Key& key) const
    {
        return *m_objects.at(key);
    }

    StoredType& getObject(const Key& key)
    {
        return *m_objects[key];
    }

    void addObject(const Key& key, std::unique_ptr<StoredType>&& object)
    {
        m_objects.emplace(key, std::move(object));
    }

private:
    std::map<Key, std::unique_ptr<StoredType>> m_objects;

};
