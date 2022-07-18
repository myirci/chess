#include "BitIndexBenchmark.hpp"
#include "BenchmarkTimer.h"
#include "Statistics.h"

#include <cmath>
#include <unordered_map>
#include <array>
#include <algorithm>
#include <random>
#include <numeric>
#include <iostream>

namespace bit_index_benchmark
{
    static constexpr uint64_t GetNum(int i) 
    {
        uint64_t num{ 1 };
        return num << i;
    }

    #define B(n) GetNum(n) 
    static constexpr uint64_t test_input[64] =
    {
        B(0), B(1), B(2), B(3), B(4), B(5), B(6), B(7), B(8), B(9), B(10), B(11), B(12), B(13), B(14), B(15),
        B(16), B(17), B(18), B(19), B(20), B(21), B(22), B(23), B(24), B(25), B(26), B(27), B(28), B(29), B(30), B(31),
        B(32), B(33), B(34), B(35), B(36), B(37), B(38), B(39), B(40), B(41), B(42), B(43), B(44), B(45), B(46), B(47),
        B(48), B(49), B(50), B(51), B(52), B(53), B(54), B(55), B(56), B(57), B(58), B(59), B(60), B(61), B(62), B(63)
    };

    static constexpr uint64_t debruijn_seq = 0x218A392CD3D5DBF;

    // for bitpos 0 : 63
    // debruijn_table [(debrujinSeq << bitpos) >> 58] = bitpos;
    static constexpr int debruijn_table[64] = 
    {
        0, 1, 2, 7, 3, 13, 8, 19, 4, 25, 14, 28, 9, 34, 20, 40, 5, 17, 26, 38, 15, 46, 29, 48, 10, 31, 35, 54, 21, 50, 41, 57,
        63, 6, 12, 18, 24, 27, 33, 39, 16, 37, 45, 47, 30, 53, 49, 56, 62, 11, 23, 32, 36, 44, 52, 55, 61, 22, 43, 51, 60, 42, 59, 58
    };

    #define A(n) {(uint64_t)exp2(n), n} 
    static const std::unordered_map<uint64_t, int> idxMap
    {
        A(0), A(1), A(2), A(3), A(4), A(5), A(6), A(7), A(8), A(9), A(10), A(11), A(12), A(13), A(14), A(15),
        A(16), A(17), A(18), A(19), A(20), A(21), A(22), A(23), A(24), A(25), A(26), A(27), A(28), A(29), A(30), A(31),
        A(32), A(33), A(34), A(35), A(36), A(37), A(38), A(39), A(40), A(41), A(42), A(43), A(44), A(45), A(46), A(47),
        A(48), A(49), A(50), A(51), A(52), A(53), A(54), A(55), A(56), A(57), A(58), A(59), A(60), A(61),  A(62), A(63)
    };

    int get_bit_index_std_log2(uint64_t num) 
    { 
        return (int)std::log2(num);
    }

    int get_bit_index_shift_left(uint64_t num)
    {
        int i{ 0 };
        while (num >>= 1)
            i++;
        return i;
    }

    int get_bit_index_unordered_map(uint64_t num)
    {
        return idxMap.at(num);
    }

    int get_bit_index_debruijn(uint64_t num)
    {
        return debruijn_table[(debruijn_seq * num) >> 58];
    }

    bool test() 
    {
        for (int i{ 0 }; i < 64; i++)
        {
            bool pass{ true };
            
            auto idx1 = get_bit_index_std_log2(test_input[i]);
            pass = pass && (idx1 == i);
            
            auto idx2 = get_bit_index_shift_left(test_input[i]);
            pass = pass && (idx2 == i);

            auto idx3 = get_bit_index_unordered_map(test_input[i]);
            pass = pass && (idx3 == i);

            auto idx4 = get_bit_index_debruijn(test_input[i]);
            pass = pass && (idx4 == i);

            if (!pass)
                return false;
        }

        return true;
    }

    double single_pass(int (*func)(uint64_t), BenchmarkTimer& timer)
    {
        std::array<int, 64> index_array{};
        std::iota(index_array.begin(), index_array.end(), 0);

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(index_array.begin(), index_array.end(), g);
        
        timer.Start();
        for (int idx : index_array) 
            func(test_input[idx]);
        timer.End();
        return timer.GetDuration();
    }

    void benchmark(int repeat)
    {
        BenchmarkTimer timer{ BenchmarkTimer::TimeUnit::nanoseconds };
        auto unit = timer.GetUnitStr();

        std::vector<double> times(repeat, 0.0);
        for (int i = 0; i < repeat; i++) 
            times[i] = single_pass(&get_bit_index_std_log2, timer);
        print_stats(times, "get_bit_index_std_log2", unit);
        std::cout << "********************************************" << std::endl;
        std::fill(times.begin(), times.end(), 0.0);

        for (int i = 0; i < repeat; i++)
            times[i] = single_pass(&get_bit_index_shift_left, timer);
        print_stats(times, "get_bit_index_shift_left", unit);
        std::cout << "********************************************" << std::endl;
        std::fill(times.begin(), times.end(), 0.0);

        for (int i = 0; i < repeat; i++)
            times[i] = single_pass(&get_bit_index_unordered_map, timer);
        print_stats(times, "get_bit_index_unordered_map", unit);
        std::cout << "********************************************" << std::endl;
        std::fill(times.begin(), times.end(), 0.0);

        for (int i = 0; i < repeat; i++)
            times[i] = single_pass(&get_bit_index_debruijn, timer);
        print_stats(times, "get_bit_index_debruijn", unit);
        std::cout << "********************************************" << std::endl;
        std::fill(times.begin(), times.end(), 0.0);
    }

    void print_stats(const std::vector<double>& data, std::string_view message, std::string_view unit)
    {
        std::cout << message << ": " << std::endl;
        std::cout << "Min   : " << Statistics<double>::GetMin(data)  << " " << unit << std::endl;
        std::cout << "Max   : " << Statistics<double>::GetMax(data) << " " << unit << std::endl;
        std::cout << "Mean  : " << Statistics<double>::GetAverage(data) << " " << unit << std::endl;
        std::cout << "Median: " << Statistics<double>::GetMedian(data) << " " << unit << std::endl;
    }
}
