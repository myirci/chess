#pragma once

#include <stack>

namespace chesslib::utility
{
    template <typename T, typename Container = std::deque<T>>
    class IterableStack : public std::stack<T>
    {
    public:

        using std::stack<T, Container>::c;

        auto bottom_to_top() const { return std::make_pair(std::begin(c), std::end(c)); }
        auto top_to_bottom() const { return std::make_pair(std::rbegin(c), std::rend(c)); }
    };
}
