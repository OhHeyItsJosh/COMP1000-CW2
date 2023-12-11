#pragma once
#include <vector>
#include <string>
#include <map>

#include "cmd/utils/cmdTagParser.h"
#include "cmd/utils/cmdTagParserResult.h"
#include "cmd/utils/cmdParseResult.h"

namespace CMDUtils
{
	CMDParseResult parseArgs(uint32_t argc, const char* argv[], std::vector<CMDTagParser> parsers);
}

