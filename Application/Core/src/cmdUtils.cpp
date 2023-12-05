#include "cmdUtils.h"

namespace CMDUtils {
	bool isTag(const char* str)
	{
		return str[0] == '-';
	}

	CMDParseResult parseArgs(uint32_t argc, const char* argv[], std::vector<CMDTagParser> parsers)
	{
		// index tag parsers by tag
		std::map<std::string, CMDTagParser> parserTagMap;
		for (CMDTagParser& parser : parsers)
		{
			parserTagMap[parser.getTag()] = parser;
		}

		std::map<std::string, CMDTagParserResult> results;
		std::vector<std::string> unrequestedArgs;

		bool requiresData;
		std::vector<std::string> inputs;

		// iterate through args
		for (uint32_t i = 0; i < argc; i++)
		{
			std::string currentArg = argv[i];

			// continue if not tag
			if (!isTag(currentArg.c_str()))
				continue;

			auto foundParser = parserTagMap.find(currentArg);
			if (foundParser == parserTagMap.end())
			{
				unrequestedArgs.push_back(currentArg);
				continue;
			}

			CMDTagParser& parser = foundParser->second;
			requiresData = parser.takesData();

			// handle data input
			if (requiresData)
			{
				uint32_t nextIndex = i + 1;
				while(nextIndex < argc && !isTag(argv[nextIndex])) 
				{
					inputs.push_back(argv[nextIndex]);
					i++;
					nextIndex++;
				}
			}

			// add result and use current parser
			results.emplace(currentArg, CMDTagParserResult( inputs, requiresData ));
			parserTagMap.erase(currentArg);
			inputs.clear();
		}

		return CMDParseResult{ results, unrequestedArgs };
	}
}
