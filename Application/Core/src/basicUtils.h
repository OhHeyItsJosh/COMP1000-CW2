#pragma once
#include <string>
#include <vector>
#include <functional>
#include <optional>

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

template<typename T, typename M>
std::optional<M> safeParse(const T& input, std::function<M(const T&)> parse)
{
	try {
		return (parse(input));
	}
	catch (std::exception e) {
		return std::nullopt;
	}
}

template<typename T, typename M>
std::vector<M> mapVector(std::vector<T> vec, std::function<M(T&)> forEach)
{
	std::vector<M> mapTo(vec.size());

	for (uint32_t i = 0; i < vec.size(); i++)
	{
		mapTo[i] = forEach(vec[i]);
	}

	return mapTo;
}