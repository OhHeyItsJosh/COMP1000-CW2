#include "cmdTagParser.h"

namespace CMDUtils
{
	CMDTagParser::CMDTagParser(std::string tag, bool takeData)
		: m_tag(tag), m_takeData(takeData)
	{}

	CMDTagParser::CMDTagParser(std::string tag)
		: m_tag(tag), m_takeData(false)
	{}

	CMDTagParser CMDTagParser::tagWithArgument(std::string tag)
	{
		return CMDTagParser(tag, true);
	}

	std::string& CMDTagParser::getTag()
	{
		return m_tag;
	}

	bool CMDTagParser::takesData()
	{
		return m_takeData;
	};
}