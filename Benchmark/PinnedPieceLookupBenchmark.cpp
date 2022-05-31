#include "PinnedPieceLookupBenchmark.h"
#include "Statistics.h"
#include <random>
#include <functional>
#include <iostream>

namespace pinned_piece_lookup_benchmark
{
	static constexpr int8_t
		a1{ 0 }, a2{ 8 }, a3{ 16 }, a4{ 24 }, a5{ 32 }, a6{ 40 }, a7{ 48 }, a8{ 56 },
		b1{ 1 }, b2{ 9 }, b3{ 17 }, b4{ 25 }, b5{ 33 }, b6{ 41 }, b7{ 49 }, b8{ 57 },
		c1{ 2 }, c2{ 10 }, c3{ 18 }, c4{ 26 }, c5{ 34 }, c6{ 42 }, c7{ 50 }, c8{ 58 },
		d1{ 3 }, d2{ 11 }, d3{ 19 }, d4{ 27 }, d5{ 35 }, d6{ 43 }, d7{ 51 }, d8{ 59 },
		e1{ 4 }, e2{ 12 }, e3{ 20 }, e4{ 28 }, e5{ 36 }, e6{ 44 }, e7{ 52 }, e8{ 60 },
		f1{ 5 }, f2{ 13 }, f3{ 21 }, f4{ 29 }, f5{ 37 }, f6{ 45 }, f7{ 53 }, f8{ 61 },
		g1{ 6 }, g2{ 14 }, g3{ 22 }, g4{ 30 }, g5{ 38 }, g6{ 46 }, g7{ 54 }, g8{ 62 },
		h1{ 7 }, h2{ 15 }, h3{ 23 }, h4{ 31 }, h5{ 39 }, h6{ 47 }, h7{ 55 }, h8{ 63 };

	void unordered_map_impl::fill(const std::vector<std::array<int8_t, 3>>& data)
	{
		for (const auto& arr : data)
			pins.insert({ arr[0], {arr[1], arr[2]} });
	}

	bool unordered_map_impl::look_up(std::int8_t sq) 
	{
		auto it = pins.find(sq);
		return it != pins.end();
	}

	void unordered_map_impl::clear()
	{
		pins.clear();
	}

	void map_impl::fill(const std::vector<std::array<int8_t, 3>>& data) 
	{
		for (const auto& arr : data)
			pins.insert({ arr[0], {arr[1], arr[2]} });
	}

	bool map_impl::look_up(std::int8_t sq) 
	{
		auto it = pins.find(sq);
		return it != pins.end();
	}

	void map_impl::clear() 
	{
		pins.clear();
	}

	void vector_tuple_impl::fill(const std::vector<std::array<int8_t, 3>>& data)
	{
		for (const auto& arr : data)
			pins.push_back(std::make_tuple(arr[0], arr[1], arr[2]));
	}

	bool vector_tuple_impl::look_up(std::int8_t sq) 
	{
		for(const auto& tp : pins)
		{
			if (std::get<0>(tp) == sq)
				return true;
		}
		return false;
	}

	void vector_tuple_impl::clear() 
	{
		pins.clear();
	}

	void vector_array_impl::fill(const std::vector<std::array<int8_t, 3>>& data) 
	{
		for (const auto& arr : data)
			pins.push_back({ arr[0], arr[1], arr[2] });
	}

	bool vector_array_impl::look_up(std::int8_t sq)
	{
		for (const auto& arr : pins)
		{
			if (arr[0] == sq)
				return true;
		}
		return false;
	}

	void vector_array_impl::clear() 
	{
		pins.clear();
	}

	void array_impl::fill(const std::vector<std::array<int8_t, 3>>& data) 
	{
		for (int i = 0; i < data.size(); i++) 
			for (int j = 0; j < 3; j++)
				pins[i][j] = data[i][j];

		num_pins = data.size();
	}

	bool array_impl::look_up(std::int8_t sq) 
	{
		for (int i = 0; i < num_pins; i++)
		{
			if (pins[i][0] == sq)
				return true;
		}
		return false;
	}
	
	void array_impl::clear() 
	{
		num_pins = 0;
	}

	void carray_impl::fill(const std::vector<std::array<int8_t, 3>>& data)
	{
		for (int i = 0; i < data.size(); i++)
			for (int j = 0; j < 3; j++)
				pins[i][j] = data[i][j];

		num_pins = data.size();
	}
	
	bool carray_impl::look_up(std::int8_t sq) 
	{
		for (int i = 0; i < num_pins; i++) 
		{
			if (pins[i][0] == sq)
				return true;
		}
		return false;
	}
	
	void carray_impl::clear() 
	{
		num_pins = 0;
	}

	void carray_struct_impl::fill(const std::vector<std::array<int8_t, 3>>& data)
	{
		for (int i = 0; i < data.size(); i++) 
		{
			pins[i].pinned = data[i][0];
			pins[i].pinner = data[i][1];
			pins[i].direction = data[i][2];
		}

		num_pins = data.size();
	}
	
	bool carray_struct_impl::look_up(std::int8_t sq)
	{
		for (int i = 0; i < num_pins; i++)
		{
			if (pins[i].pinned == sq)
				return true;
		}
		return false;
	}
	
	void carray_struct_impl::clear()
	{
		num_pins = 0;
	}

	void vector_struct_impl::fill(const std::vector<std::array<int8_t, 3>>& data) 
	{
		for (const auto& arr : data)
			pins.push_back(pin_struct(arr[0], arr[1], arr[2]));
	}
	
	bool vector_struct_impl::look_up(std::int8_t sq) 
	{
		for (const auto& ps : pins)
			if (ps.pinned == sq)
				return true;
		return false;
	}
	
	void vector_struct_impl::clear() 
	{
		pins.clear();
	}

	bool test()
	{
		std::vector<std::array<int8_t, 3>> data 
		{
			{a4, c6, 9}, {g4, e7, 8}, {c1, g5, 7}, {d7, a4, 1}, {h2, g4, -1}, {f2, b1, -9}, {e3, e1, -8}, {b8, g2, -9}
		};

		std::array<bool, 64> expected_res{};
		expected_res.fill(false);
		expected_res[a4] = true;
		expected_res[g4] = true;
		expected_res[c1] = true;
		expected_res[d7] = true;
		expected_res[h2] = true;
		expected_res[f2] = true;
		expected_res[e3] = true;
		expected_res[b8] = true;

		unordered_map_impl imp1;
		imp1.fill(data);
		for (auto i = 0; i < 64; i++)
			if (expected_res[i] != imp1.look_up(i))
				return false;

		map_impl imp2;
		imp2.fill(data);
		for (auto i = 0; i < 64; i++)
			if (expected_res[i] != imp2.look_up(i))
				return false;

		vector_tuple_impl imp3;
		imp3.fill(data);
		for (auto i = 0; i < 64; i++)
			if (expected_res[i] != imp3.look_up(i))
				return false;

		vector_array_impl imp4;
		imp4.fill(data);
		for (auto i = 0; i < 64; i++)
			if (expected_res[i] != imp4.look_up(i))
				return false;

		array_impl imp5;
		imp5.fill(data);
		for (auto i = 0; i < 64; i++)
			if (expected_res[i] != imp5.look_up(i))
				return false;

		carray_impl imp6;
		imp6.fill(data);
		for (auto i = 0; i < 64; i++)
			if (expected_res[i] != imp6.look_up(i))
				return false;

		carray_struct_impl imp7;
		imp7.fill(data);
		for (auto i = 0; i < 64; i++)
			if (expected_res[i] != imp7.look_up(i))
				return false;

		vector_struct_impl imp8;
		imp8.fill(data);
		for (auto i = 0; i < 64; i++)
			if (expected_res[i] != imp8.look_up(i))
				return false;

		return true;
	}

	void print_stats(const std::vector<double>& data, std::string_view message, std::string_view unit)
	{
		std::cout << std::endl;
		std::cout << message << ": " << std::endl;
		std::cout << "Min   : " << Statistics<double>::GetMin(data) << " " << unit << std::endl;
		std::cout << "Max   : " << Statistics<double>::GetMax(data) << " " << unit << std::endl;
		std::cout << "Mean  : " << Statistics<double>::GetAverage(data) << " " << unit << std::endl;
		std::cout << "Median: " << Statistics<double>::GetMedian(data) << " " << unit << std::endl;
	}

	std::pair<double, double> single_pass(
		std::function<void(const std::vector<std::array<int8_t, 3>>&)> fill, 
		std::function<bool(int8_t)> lookup, 
		BenchmarkTimer& timer)
	{
		std::default_random_engine generator;
		std::uniform_int_distribution<int> size_distribution(1, 8);
		// int roll = size_distribution(generator);
		int roll = 3;

		std::uniform_int_distribution<int> square_distribution(0, 63);

		std::vector<std::array<int8_t, 3>> data(roll, { 0, 0, 0 });
		for (int i = 0; i < roll; i++) 
			data[0][0] = square_distribution(generator);

		std::array<int, 64> index_array{};
		std::iota(index_array.begin(), index_array.end(), 0);
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(index_array.begin(), index_array.end(), g);

		timer.Start();
		fill(data);
		timer.End();
		auto d1 = timer.GetDuration();
		
		timer.Start();
		for (int idx : index_array)
			lookup(idx);
		timer.End();
		auto d2 = timer.GetDuration();
		
		return std::make_pair(d1, d2);	
	}


	void benchmark(int repeat)
	{
		BenchmarkTimer timer{ BenchmarkTimer::TimeUnit::nanoseconds };
		auto unit = timer.GetUnitStr();

		// unordered_map_impl
		{
			unordered_map_impl imp1;
			std::vector<double> fill_times(repeat, 0.0), lookup_times(repeat, 0.0), total_times(repeat, 0.0);
			for (int i = 0; i < repeat; i++)
			{
				auto [tf, tl] = single_pass(
					std::bind(&unordered_map_impl::fill, &imp1, std::placeholders::_1),
					std::bind(&unordered_map_impl::look_up, &imp1, std::placeholders::_1),
					timer);
				fill_times[i] = tf;
				lookup_times[i] = tl;
				total_times[i] = tl + tf;
			}

			// print_stats(fill_times, "unordered_map_impl-fill", unit);
			// print_stats(lookup_times, "unordered_map_impl-lookup", unit);
			print_stats(total_times, "unordered_map_impl-total", unit);
			std::cout << "********************************************" << std::endl;
		}
		

		// map_impl
		{
			map_impl imp2;
			std::vector<double> fill_times(repeat, 0.0), lookup_times(repeat, 0.0), total_times(repeat, 0.0);
			for (int i = 0; i < repeat; i++)
			{
				auto [tf, tl] = single_pass(
					std::bind(&map_impl::fill, &imp2, std::placeholders::_1),
					std::bind(&map_impl::look_up, &imp2, std::placeholders::_1),
					timer);
				fill_times[i] = tf;
				lookup_times[i] = tl;
				total_times[i] = tl + tf;
			}

			// print_stats(fill_times, "map_impl-fill", unit);
			// print_stats(lookup_times, "map_impl-lookup", unit);
			print_stats(total_times, "map_impl-total", unit);
			std::cout << "********************************************" << std::endl;
		}

		// vector_tuple_imp
		{
			vector_tuple_impl imp3;
			std::vector<double> fill_times(repeat, 0.0), lookup_times(repeat, 0.0), total_times(repeat, 0.0);
			for (int i = 0; i < repeat; i++)
			{
				auto [tf, tl] = single_pass(
					std::bind(&vector_tuple_impl::fill, &imp3, std::placeholders::_1),
					std::bind(&vector_tuple_impl::look_up, &imp3, std::placeholders::_1),
					timer);
				fill_times[i] = tf;
				lookup_times[i] = tl;
				total_times[i] = tl + tf;
			}

			// print_stats(fill_times, "vector_tuple_impl-fill", unit);
			// print_stats(lookup_times, "vector_tuple_impl-lookup", unit);
			print_stats(total_times, "vector_tuple_impl-total", unit);
			std::cout << "********************************************" << std::endl;
		}
		
		// vector_array_impl
		{
			vector_array_impl imp4;
			std::vector<double> fill_times(repeat, 0.0), lookup_times(repeat, 0.0), total_times(repeat, 0.0);
			for (int i = 0; i < repeat; i++)
			{
				auto [tf, tl] = single_pass(
					std::bind(&vector_array_impl::fill, &imp4, std::placeholders::_1),
					std::bind(&vector_array_impl::look_up, &imp4, std::placeholders::_1),
					timer);
				fill_times[i] = tf;
				lookup_times[i] = tl;
				total_times[i] = tl + tf;
			}

			// print_stats(fill_times, "vector_array_impl-fill", unit);
			// print_stats(lookup_times, "vector_array_impl-lookup", unit);
			print_stats(total_times, "vector_array_impl-total", unit);
			std::cout << "********************************************" << std::endl;
		}
		
		// array_impl
		{
			array_impl imp5;
			std::vector<double> fill_times(repeat, 0.0), lookup_times(repeat, 0.0), total_times(repeat, 0.0);
			for (int i = 0; i < repeat; i++)
			{
				auto [tf, tl] = single_pass(
					std::bind(&array_impl::fill, &imp5, std::placeholders::_1),
					std::bind(&array_impl::look_up, &imp5, std::placeholders::_1),
					timer);
				fill_times[i] = tf;
				lookup_times[i] = tl;
				total_times[i] = tl + tf;
			}

			// print_stats(fill_times, "array_impl-fill", unit);
			// print_stats(lookup_times, "array_impl-lookup", unit);
			print_stats(total_times, "array_impl-total", unit);
			std::cout << "********************************************" << std::endl;
		}

		// carray_impl
		{
			carray_impl imp;
			std::vector<double> fill_times(repeat, 0.0), lookup_times(repeat, 0.0), total_times(repeat, 0.0);
			for (int i = 0; i < repeat; i++)
			{
				auto [tf, tl] = single_pass(
					std::bind(&carray_impl::fill, &imp, std::placeholders::_1),
					std::bind(&carray_impl::look_up, &imp, std::placeholders::_1),
					timer);
				fill_times[i] = tf;
				lookup_times[i] = tl;
				total_times[i] = tl + tf;
			}

			// print_stats(fill_times, "carray_impl-fill", unit);
			// print_stats(lookup_times, "carray_impl-lookup", unit);
			print_stats(total_times, "carray_impl-total", unit);
			std::cout << "********************************************" << std::endl;
		}

		// carray_struct_impl
		{
			carray_struct_impl imp;
			std::vector<double> fill_times(repeat, 0.0), lookup_times(repeat, 0.0), total_times(repeat, 0.0);
			for (int i = 0; i < repeat; i++)
			{
				auto [tf, tl] = single_pass(
					std::bind(&carray_struct_impl::fill, &imp, std::placeholders::_1),
					std::bind(&carray_struct_impl::look_up, &imp, std::placeholders::_1),
					timer);
				fill_times[i] = tf;
				lookup_times[i] = tl;
				total_times[i] = tl + tf;
			}

			// print_stats(fill_times, "carray_struct_impl-fill", unit);
			// print_stats(lookup_times, "carray_struct_impl-lookup", unit);
			print_stats(total_times, "carray_struct_impl-total", unit);
			std::cout << "********************************************" << std::endl;
		}

		// vector_struct_impl
		{
			vector_struct_impl imp;
			std::vector<double> fill_times(repeat, 0.0), lookup_times(repeat, 0.0), total_times(repeat, 0.0);
			for (int i = 0; i < repeat; i++)
			{
				auto [tf, tl] = single_pass(
					std::bind(&vector_struct_impl::fill, &imp, std::placeholders::_1),
					std::bind(&vector_struct_impl::look_up, &imp, std::placeholders::_1),
					timer);
				fill_times[i] = tf;
				lookup_times[i] = tl;
				total_times[i] = tl + tf;
			}

			// print_stats(fill_times, "vector_struct_impl-fill", unit);
			// print_stats(lookup_times, "vector_struct_impl-lookup", unit);
			print_stats(total_times, "vector_struct_impl-total", unit);
			std::cout << "********************************************" << std::endl;
		}
	}
}
