#pragma once

namespace sft
{

class Config
{
public:
	Config &singleton();

	std::string &searchPaths() const				{ return m_searchPaths; }

	void setSearchPaths(const std::string &paths)	{ m_searchPaths = paths; }

private:
	Config();

	~Config();

	std::string m_searchPaths;
}

}	//namespace sft
