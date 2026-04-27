#ifndef SORTING_H
#define SORTING_H

// Header-only

// Для swap
#include "collvalue.h"

// ----------------------------- Пузырёк (например) --------------------------------------
template <typename Iterator>
void bubble_sort(Iterator begin, Iterator end)
{
    if (begin == end) {
        return;
    }

    for (auto i = begin; i != end; ++i) {
        for (auto j = begin; j != end - 1; ++j) {
            if (*(j + 1) < *j) {
                using std::swap;
                swap(*j, *(j + 1));
            }
        }
    }
}

// --------------------------- QuickSort (например) ---------------------------------------
template <typename Iterator>
Iterator separation(Iterator left, Iterator right) {
    auto opr = *(right - 1);
    auto i = left;

    for (auto j = left; j != right - 1; ++j) {
        if (!(opr < *j)) {
            using std::swap;
            swap(*i, *j);
            ++i;
        }
    }

    using std::swap;
    swap(*i, *(right - 1));
    return i;
}

template <typename Iterator>
void quick_sort_impl(Iterator left, Iterator right) {
    if (left < right - 1) {
        auto fence = separation(left, right);
        quick_sort_impl(left, fence);
        quick_sort_impl(fence + 1, right);
    }
}

template <typename Iterator>
void quick_sort(Iterator begin, Iterator end)
{
    if (begin < end) {
        quick_sort_impl(begin, end);
    }
}

#endif // SORTING_H
