#pragma once

#include <vector>
#include <algorithm>
#include <numeric>

template <typename T>
class Statistics
{
public:
	static T GetAverage(const std::vector<T>& data) 
	{
		auto sum = std::accumulate(data.begin(), data.end(), 0.0);
		return static_cast<T>(sum / data.size());
	}

	static T GetMedian(const std::vector<T>& data)
	{
		return data[data.size() / 2];
	}

	static T GetMin(const std::vector<T>& data)
	{
		auto it = std::min_element(data.begin(), data.end());
		return *it;
	}

	static T GetMax(const std::vector<T>& data)
	{
		auto it = std::max_element(data.begin(), data.end());
		return *it;
	}
};

