#include "cmdParseResult.h"

namespace CMDUtils
{
	CMDParseResult::CMDParseResult(std::map<std::string, CMDTagParserResult> results, std::vector<std::string> unrequestedInputs)
		: m_results(results), m_unrequestedInputs(unrequestedInputs)
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

	bool CMDParseResult::hasUnrequestedArgs()
	{
		return m_unrequestedInputs.size() > 0;
	}

	std::vector<std::string>& CMDParseResult::getUnrequestedArgs()
	{
		return m_unrequestedInputs;
	}
}