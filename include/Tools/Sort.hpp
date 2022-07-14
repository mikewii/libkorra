#pragma once

// operating class must have method <friend void swap(T& a, T& b)>
template<typename T, typename Comparator>
void sort_swap_only(T begin, T end, const Comparator comp)
{
    if (begin == end)
        return;

    for (auto& left = begin; begin != end; left++)
        for (auto right = left; right != end; ++right)
            if (comp(*left, *right))
                swap(*left, *right);
}
