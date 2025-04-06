#pragma once

namespace sft
{

class Config
{
public:
	static Config &singleton();

	std::string &searchPaths() const				{ return _searchPaths; }

	void setSearchPaths(const std::string &paths)	{ _searchPaths = paths; }

private:
	Config();

	~Config();

	std::string _searchPaths;
}

}	//namespace sft
