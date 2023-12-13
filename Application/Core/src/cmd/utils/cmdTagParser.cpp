#include "cmdTagParser.h"

namespace CMDUtils
{
	CMDTagParser::CMDTagParser(const std::string& tag, bool takeData, uint32_t minArgCount, uint32_t maxArgCount)
		: m_tag(tag), m_takeData(takeData), m_minArgCount(minArgCount), m_maxArgCount(maxArgCount)
	{}

	CMDTagParser::CMDTagParser(const std::string& tag)
		: m_tag(tag), m_takeData(false), m_minArgCount(0), m_maxArgCount(0)
	{}

	CMDTagParser CMDTagParser::tagWithArgument(const std::string& tag)
	{
		return CMDTagParser(tag, true, 1, 1);
	}

	CMDTagParser CMDTagParser::tagWithMultipleArguments(const std::string& tag, uint32_t minArgs, uint32_t maxArgs)
	{
		return CMDTagParser(tag, true, minArgs, maxArgs);
	}

	std::string& CMDTagParser::getTag()
	{
		return m_tag;
	}

	uint32_t CMDTagParser::getMinArgCount()
	{
		return m_minArgCount;
	}

	uint32_t CMDTagParser::getMaxArgCount()
	{
		return m_maxArgCount;
	}

	bool CMDTagParser::takesData()
	{
		return m_takeData;
	};
}