#pragma once

namespace sft
{

class Config
{
public:
	Config &singleton();

	std::string &searchPaths() const				{ return _searchPaths; }

	void setSearchPaths(const std::string &paths)	{ _searchPaths = paths; }

private:
	Config();

	~Config();

	std::string _searchPaths;
}

}	//namespace sft
