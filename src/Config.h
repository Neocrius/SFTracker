#pragma once

#include <string>

namespace sft
{

class Config
{
public:
	static Config &singleton();

	const std::string &searchPaths() const			{ return _searchPaths; }

	void setSearchPaths(const std::string &paths)	{ _searchPaths = paths; }

	//TODO: store GUI theme
private:
	Config();

	~Config();

	std::string _searchPaths;
};

}	//namespace sft
