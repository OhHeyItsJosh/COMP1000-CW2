#include "cmdParseResult.h"
#include "../../basicUtils.h"
#include <iostream>
#include <sstream>

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

	void CMDParseResult::logUnrequestedArgs(std::ostream& stream)
	{
		// print out the provided args that are not recognised
		stream << "The following arguments could not be recognised or are duplicates of existing arguments:\n -> "
			<< stringifyList<std::string>(this->getUnrequestedArgs(), [](const std::string& item, bool last, std::stringstream& builder) {
				builder << "\"" << item << "\"" << (last ? "" : ", ");
			}) << std::endl;
	}
}