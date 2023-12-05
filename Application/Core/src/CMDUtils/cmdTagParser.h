#pragma once

#include <string>

namespace CMDUtils 
{
	class CMDTagParser
	{
	private:
		std::string m_tag;
		bool m_takeData;

	public:
		CMDTagParser() = default;
		CMDTagParser(std::string tag,  bool takeData);
		CMDTagParser(std::string tag);
		static CMDTagParser tagWithArgument(std::string tag);

	public:
		std::string& getTag();
		bool takesData();
	};
}
