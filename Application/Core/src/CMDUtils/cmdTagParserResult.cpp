#include "cmdTagParserResult.h"
#include <iostream>

namespace CMDUtils
{
	CMDTagParserResult::CMDTagParserResult(std::vector<std::string> inputs, CMDTagParser&& parser)
		: inputs(inputs), m_parser(parser)
	{}

	bool CMDTagParserResult::isValid()
	{
		if (m_parser.takesData())
		{
			size_t inputSize = this->inputs.size();
			return inputSize >= m_parser.getMinArgCount() && inputSize <= m_parser.getMaxArgCount();
		}

		return this->inputs.size() == 0;
	}

	#define ARGUMENT_FROM_COUNT(count) ( count == 1 ? " argument" : "arguments" )

	// does what the name suggests, output is formatted to best suit the min and max
	void CMDTagParserResult::logArgCount(std::ostream& outStream)
	{
		outStream << "Tag '" << m_parser.getTag() << "' requires ";

		if (m_parser.getMaxArgCount() == -1)
			outStream << "at least " << m_parser.getMinArgCount() << ARGUMENT_FROM_COUNT(m_parser.getMinArgCount());

		else if (m_parser.getMaxArgCount() == m_parser.getMinArgCount())
			outStream << m_parser.getMinArgCount() << ARGUMENT_FROM_COUNT(m_parser.getMinArgCount());

		else
			outStream << m_parser.getMinArgCount() << " - " << m_parser.getMaxArgCount() << " " << ARGUMENT_FROM_COUNT(2);
		
		outStream << " (" << this->inputs.size() << " provided)" << std::endl;
	}

	const std::string& CMDTagParserResult::getSingletonInput()
	{
		return inputs[0];
	}
}