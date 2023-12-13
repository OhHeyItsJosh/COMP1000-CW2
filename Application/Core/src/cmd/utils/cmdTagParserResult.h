#pragma once
#include <string>
#include <vector>

#include "cmdTagParser.h"

#define ENSURE_ARG_VALID(arg, returnVal) if (!arg->isValid()) \
{ \
	if (arg) arg->logArgCount(std::cout); \
	return returnVal; \
}

#define ENSURE_REQUIRED_ARG_VALID(arg, argName, argHint, returnVal) if (arg == nullptr) \
{ \
	printf("%s parameter is required: '%s'", argName, argHint); \
	return returnVal; \
} \
else { \
	ENSURE_ARG_VALID(arg, returnVal); \
}

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