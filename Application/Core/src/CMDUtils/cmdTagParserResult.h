#pragma once
#include <string>
#include <vector>

namespace CMDUtils
{
	class CMDTagParserResult
	{
	public:
		const std::vector<std::string> inputs;
		const bool requiresValue;
		const bool hasValue;

	public:
		CMDTagParserResult(std::vector<std::string> inputs, bool required);

	public:
		bool isValid();
		const std::string& getSingletonInput();
	};
}