#include "cmdTagParserResult.h"

namespace CMDUtils
{
	CMDTagParserResult::CMDTagParserResult(std::string value, bool required, bool hasValue)
		: value(value), requiresValue(required), hasValue(hasValue)
	{}

	bool CMDTagParserResult::isValid()
	{
		if (this->requiresValue)
			return this->hasValue;

		return true;
	}
}