#include "cmdTagParserResult.h"

namespace CMDUtils
{
	CMDTagParserResult::CMDTagParserResult(std::vector<std::string> inputs, bool required)
		: inputs(inputs), requiresValue(required), hasValue(inputs.size() > 0)
	{}

	bool CMDTagParserResult::isValid()
	{
		if (this->requiresValue)
			return this->hasValue;

		return true;
	}

	const std::string& CMDTagParserResult::getSingletonInput()
	{
		return inputs[0];
	}
}