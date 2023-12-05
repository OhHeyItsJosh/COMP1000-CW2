#include "cmdUtils.h"

namespace CMDUtils {
	CMDParseResult parseArgs(uint32_t argc, const char* argv[], std::vector<CMDTagParser> parsers)
	{
		// index tag parsers by tag
		std::map<std::string, CMDTagParser> parserTagMap;
		for (CMDTagParser& parser : parsers)
		{
			parserTagMap[parser.getTag()] = parser;
		}

		std::map<std::string, CMDTagParserResult> results;

		bool hasData, requiresData;
		std::string nextValue;

		// iterate through args
		for (uint32_t i = 0; i < argc; i++)
		{
			std::string currentArg = argv[i];

			// continue if not tag
			if (parserTagMap.count(currentArg) == 0)
				continue;

			CMDTagParser& parser = parserTagMap[currentArg];
			requiresData = parser.takesData();

			// handle data input
			if (parserTagMap[currentArg].takesData())
			{
				uint32_t nextIndex = i + 1;
				hasData = nextIndex < argc && (parserTagMap.find(argv[nextIndex]) == parserTagMap.end());

				if (hasData) {
					nextValue = argv[nextIndex];
					i++;
				}
			}
			else
				hasData = false;

			// add result and use current parser
			results.emplace(currentArg, CMDTagParserResult( nextValue, requiresData, hasData ));
			parserTagMap.erase(currentArg);
		}

		return CMDParseResult{ results };
	}
}
