#pragma once

#include <string>

namespace Utils
{
    template <typename T>
    std::string toString(T val);

    template <typename Container>
    bool containersEqual(const Container& first, const Container& second)
    {
        if (first.size() != second.size()) return false;

        for (size_t i = 0; i != first.size(); ++i)
        {
            if (first[i] != second[i]) return false;
        }

        return true;
    }
}
