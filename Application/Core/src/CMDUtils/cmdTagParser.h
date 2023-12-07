#pragma once

#include <string>

namespace CMDUtils 
{
	class CMDTagParser
	{
	private:
		std::string m_tag;
		bool m_takeData;
		uint32_t m_maxArgCount;
		uint32_t m_minArgCount;

	public:
		CMDTagParser() = default;
		CMDTagParser(const std::string& tag, bool takeData, uint32_t minArgCount, uint32_t maxArgCount);
		CMDTagParser(const std::string& tag);
		static CMDTagParser tagWithArgument(const std::string& tag);
		static CMDTagParser tagWithMultipleArguments(const std::string& tag, uint32_t minArgs, uint32_t maxArgs);

	public:
		std::string& getTag();
		uint32_t getMinArgCount();
		uint32_t getMaxArgCount();
		bool takesData();
	};
}
