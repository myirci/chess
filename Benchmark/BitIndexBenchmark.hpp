#pragma once

#include <cstdint>
#include <vector>
#include <string_view>

class BenchmarkTimer;

namespace bit_index_benchmark 
{
    /*
        Different methods to compute the index of a 64-bit word which is equal to a power of 2.
    */

    inline int get_bit_index_std_log2(uint64_t);
    inline int get_bit_index_shift_left(uint64_t);
    inline int get_bit_index_unordered_map(uint64_t);
    inline int get_bit_index_debruijn_msb(uint64_t);
    inline int get_bit_index_debruijn_lsb(uint64_t);
 
    bool test();
    double single_pass(int (*func)(uint64_t), BenchmarkTimer&);
    void benchmark(int);
    void print_stats(const std::vector<double>&, std::string_view, std::string_view);
}

