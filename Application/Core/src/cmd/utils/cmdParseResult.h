#pragma once
#include <map>
#include <string>
#include <vector>
#include "cmdTagParserResult.h"

namespace CMDUtils
{
	class CMDParseResult
	{
	private:
		std::map<std::string, CMDTagParserResult> m_results;
		std::vector<std::string> m_unrequestedInputs;

	public:
		CMDParseResult(std::map<std::string, CMDTagParserResult> results, std::vector<std::string> unrequestedInputs);

	public:
		CMDTagParserResult* getResult(const std::string& tag);
		bool hasResult(const std::string& tag);
		bool hasUnrequestedArgs();
		std::vector<std::string>& getUnrequestedArgs();
	};
}
