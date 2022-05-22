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

    static constexpr uint64_t debruijn_seq = 0x03f6eaf2cd271461;

    static constexpr int debruijn_table_msb[64] =
    {
        0, 58, 1, 59, 47, 53, 2, 60, 39, 48, 27, 54, 33, 42, 3, 61,
        51, 37, 40, 49, 18, 28, 20, 55, 30, 34, 11, 43, 14, 22, 4, 62,
        57, 46, 52, 38, 26, 32, 41, 50, 36, 17, 19, 29, 10, 13, 21, 56,
        45, 25, 31, 35, 16, 9, 12, 44, 24, 15, 8, 23, 7, 6, 5, 63
    };

    static constexpr int GetDebruijnIndex(uint64_t debrujinSeq, int bitpos)
    {
        // index of the table for value i, i.e. debruijn_table_lsb[(debrujinSeq << bitpos) >> 58] = bitpos;
        return static_cast<int>((debrujinSeq << bitpos) >> 58);
    }

    #define C(n) GetDebruijnIndex(debruijn_seq, n)
    static constexpr int debruijn_table_lsb[64] = 
    {
        C(0), C(1), C(2), C(3), C(4), C(5), C(6), C(7), C(8), C(9), C(10), C(11), C(12), C(13), C(14), C(15),
        C(16), C(17), C(18), C(19), C(20), C(21), C(22), C(23), C(24), C(25), C(26), C(27), C(28), C(29), C(30), C(31),
        C(32), C(33), C(34), C(35), C(36), C(37), C(38), C(39), C(40), C(41), C(42), C(43), C(44), C(45), C(46), C(47),
        C(48), C(49), C(50), C(51), C(52), C(53), C(54), C(55), C(56), C(57), C(58), C(59), C(60), C(61),  C(62), C(63)
    };

    #define A(n) {exp2(n), n} 
    static const std::unordered_map<uint64_t, int> idxMap
    {
        A(0), A(1), A(2), A(3), A(4), A(5), A(6), A(7), A(8), A(9), A(10), A(11), A(12), A(13), A(14), A(15),
        A(16), A(17), A(18), A(19), A(20), A(21), A(22), A(23), A(24), A(25), A(26), A(27), A(28), A(29), A(30), A(31),
        A(32), A(33), A(34), A(35), A(36), A(37), A(38), A(39), A(40), A(41), A(42), A(43), A(44), A(45), A(46), A(47),
        A(48), A(49), A(50), A(51), A(52), A(53), A(54), A(55), A(56), A(57), A(58), A(59), A(60), A(61),  A(62), A(63)
    };

    int get_bit_index_std_log2(uint64_t num) 
    { 
        return std::log2(num);
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

    int get_bit_index_debruijn_msb(uint64_t num)
    {
        num |= num >> 1;
        num |= num >> 2;
        num |= num >> 4;
        num |= num >> 8;
        num |= num >> 16;
        num |= num >> 32;
        
        return debruijn_table_msb[(num * debruijn_seq) >> 58];
    }

    int get_bit_index_debruijn_lsb(uint64_t num)
    {
        auto x = num * debruijn_seq;
        x >>= 58;

        return debruijn_table_lsb[x];
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

            auto idx4 = get_bit_index_debruijn_msb(test_input[i]);
            pass = pass && (idx4 == i);

            auto idx5 = get_bit_index_debruijn_lsb(test_input[i]);
            pass = pass && (idx5 == i);

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
            times[i] = single_pass(&get_bit_index_debruijn_msb, timer);
        print_stats(times, "get_bit_index_debruijn_msb", unit);
        std::cout << "********************************************" << std::endl;
        std::fill(times.begin(), times.end(), 0.0);

        for (int i = 0; i < repeat; i++)
            times[i] = single_pass(&get_bit_index_debruijn_lsb, timer);
        print_stats(times, "get_bit_index_debruijn_lsb", unit);
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
