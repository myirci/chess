#pragma once

#include <unordered_map>
#include <vector>
#include <array>
#include <tuple>
#include <map>
#include <functional>

#include "BenchmarkTimer.h"

namespace pinned_piece_lookup_benchmark
{
	// There can be at most 8 pinned piece from 8 different directions

	struct unordered_map_impl 
	{
		// key: pinned piece location
		// value.first: attacker location
		// value.second: direction from pinned piece to the attacker
		std::unordered_map<int8_t, std::pair<int8_t, int8_t>> pins;

		void fill(const std::vector<std::array<int8_t, 3>>& data);
		bool look_up(int8_t sq);
		void clear();
	};

	struct map_impl
	{
		// key: pinned piece location
		// value.first: attacker location
		// value.second: direction from pinned piece to the attacker
		std::map<int8_t, std::pair<int8_t, int8_t>> pins;
		
		void fill(const std::vector<std::array<int8_t, 3>>& data);
		bool look_up(int8_t sq);
		void clear();
	};

	struct vector_tuple_impl
	{
		// 1st: pinned piece location, 
		// 2nd: attacker location,
		// 3rd: direction from pinned piece to the attacker
		std::vector<std::tuple<int8_t, int8_t, int8_t>> pins;
		
		void fill(const std::vector<std::array<int8_t, 3>>& data);
		bool look_up(int8_t sq);
		void clear();
	};

	struct vector_array_impl
	{
		// 1st: pinned piece location, 
		// 2nd: attacker location,
		// 3rd: direction from pinned piece to the attacker
		std::vector<std::array<int8_t, 3>> pins;

		void fill(const std::vector<std::array<int8_t, 3>>& data);
		bool look_up(int8_t sq);
		void clear();
	};

	struct array_impl
	{
		// 1st: pinned piece location, 
		// 2nd: attacker location,
		// 3rd: direction from pinned piece to the attacker
		std::array<std::array<int8_t, 3>, 8> pins;
		int num_pins;

		void fill(const std::vector<std::array<int8_t, 3>>& data);
		bool look_up(int8_t sq);
		void clear();
	};

	struct carray_impl
	{
		// 1st: pinned piece location, 
		// 2nd: attacker location,
		// 3rd: direction from pinned piece to the attacker
		int8_t pins[8][3];
		int num_pins;

		void fill(const std::vector<std::array<int8_t, 3>>& data);
		bool look_up(int8_t sq);
		void clear();
	};

	struct pin_struct
	{
		pin_struct() : pinned{ 0 }, pinner{ 0 }, direction{ 0 } { }
		pin_struct(int8_t a, int8_t b, int8_t c) : pinned{ a }, pinner{ b }, direction{ c } { }
		int8_t pinned;
		int8_t pinner;
		int8_t direction;
	};

	struct carray_struct_impl
	{
		pin_struct pins[8];
		int num_pins;

		void fill(const std::vector<std::array<int8_t, 3>>& data);
		bool look_up(int8_t sq);
		void clear();
	};

	struct vector_struct_impl
	{
		std::vector<pin_struct> pins;

		void fill(const std::vector<std::array<int8_t, 3>>& data);
		bool look_up(int8_t sq);
		void clear();
	};

	bool test();
	std::pair<double, double> single_pass(
		std::function<void(const std::vector<std::array<int8_t, 3>>&)> fill,
		std::function<bool(int8_t)> lookup,
		BenchmarkTimer& timer);
	void benchmark(int repetition);
}