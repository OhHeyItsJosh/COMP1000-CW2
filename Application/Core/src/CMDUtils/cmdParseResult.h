#pragma once
#include <map>
#include <string>
#include "cmdTagParserResult.h"

namespace CMDUtils
{
	class CMDParseResult
	{
	private:
		std::map<std::string, CMDTagParserResult> m_results;

	public:
		CMDParseResult(std::map<std::string, CMDTagParserResult> results);

	public:
		CMDTagParserResult* getResult(const std::string& tag);
		bool hasResult(const std::string& tag);
	};
}
