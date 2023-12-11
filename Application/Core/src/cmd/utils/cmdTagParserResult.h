#pragma once
#include <string>
#include <vector>

#include "cmdTagParser.h"

namespace CMDUtils
{
	class CMDTagParserResult
	{
	private:
		CMDTagParser m_parser;

	public:
		const std::vector<std::string> inputs;

	public:
		CMDTagParserResult(std::vector<std::string> inputs, CMDTagParser&& parser);

	public:
		bool isValid();
		void logArgCount(std::ostream& outStream);
		const std::string& getSingletonInput();
	};
}