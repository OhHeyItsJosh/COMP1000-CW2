#pragma once
#include <string>
#include <functional>

//template<typename T>
//class SerialisationHandler
//{
//private:
//	std::function<T(std::string)> m_onDeserialise;
//	std::function<std::string(T)> m_onSerialise;
//
//private:
//	static T standardDeserialise(std::string input)
//	{
//		return (T)input;
//	}
//
//	static std::string standardSerialise(T input)
//	{
//		return std::string() << input;
//	}
//
//public:
//	SerialisationHandler(std::function<std::string(T)> onSerialise, std::function<T(std::string)> onDeserialise);
//	SerialisationHandler();
//
//public:
//	T deserialise(std::string input) { m_onDeserialise(input); }
//	std::string serialise(T input) { m_onSerialise(input); }
//};
