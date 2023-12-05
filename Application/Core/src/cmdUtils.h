#pragma once
#include <vector>
#include <string>
#include <map>

#include "CMDUtils/cmdTagParser.h"
#include "CMDUtils/cmdTagParserResult.h"
#include "CMDUtils/cmdParseResult.h"

namespace CMDUtils
{
	CMDParseResult parseArgs(uint32_t argc, const char* argv[], std::vector<CMDTagParser> parsers);
}

