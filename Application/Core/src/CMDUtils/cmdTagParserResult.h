#pragma once
#include <string>

namespace CMDUtils
{
	class CMDTagParserResult
	{
	public:
		const std::string value;
		const bool requiresValue;
		const bool hasValue;

	public:
		CMDTagParserResult(std::string value, bool required, bool hasValue);

	public:
		bool isValid();
	};
}