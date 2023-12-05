#pragma once
#include <string>
#include <vector>
#include <functional>

// The body for these 2 functions needs to be defined in the header file because they use templates.
template<typename T>
std::string stringifyList(std::vector<T> list, std::function<void(const T&, bool, std::stringstream&)> forEach)
{
	std::stringstream builder{};

	for (uint32_t i = 0; i < list.size(); i++)
	{
		const T& item = list[i];
		forEach(item, i + 1 == list.size(), builder);
	}

	return builder.str();
}

template<typename T>
std::string defaultStringifyList(std::vector<T> list)
{
	return stringifyList<T>(list, [](const T& item, bool last, std::stringstream& builder) {
		builder << item << (last ? "" : " ");
		});
}