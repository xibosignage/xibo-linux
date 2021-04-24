#pragma once

#include <deque>

namespace Stats
{
    template <typename T>
    class Container
    {
        using container = std::deque<T>;

    public:
        using iterator = typename container::iterator;
        using const_iterator = typename container::const_iterator;

    public:
        void add(const T& record)
        {
            records_.emplace_back(record);
        }

        void add(T&& record)
        {
            records_.emplace_back(std::move(record));
        }

        void clear()
        {
            records_.clear();
        }

        void remove(iterator begin, size_t count)
        {
            auto end = std::next(begin, count);
            records_.erase(begin, end);
        }

        size_t size() const
        {
            return records_.size();
        }

        bool empty() const
        {
            return records_.empty();
        }

        iterator begin()
        {
            return records_.begin();
        }

        iterator end()
        {
            return records_.end();
        }

        const_iterator begin() const
        {
            return records_.begin();
        }

        const_iterator end() const
        {
            return records_.end();
        }

    private:
        std::deque<T> records_;
    };

}
