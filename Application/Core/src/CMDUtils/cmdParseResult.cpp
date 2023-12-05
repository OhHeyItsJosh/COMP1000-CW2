#include "cmdParseResult.h"

namespace CMDUtils
{
	CMDParseResult::CMDParseResult(std::map<std::string, CMDTagParserResult> results)
		: m_results(results)
	{}

	CMDTagParserResult* CMDParseResult::getResult(const std::string& tag)
	{
		bool included = this->hasResult(tag);
		if (!included)
			return nullptr;

		return &m_results.at(tag);
	}

	bool CMDParseResult::hasResult(const std::string& tag)
	{
		return m_results.find(tag) != m_results.end();
	}
}