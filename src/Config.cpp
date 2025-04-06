#include "Config.h"
#include <SCL.h>

using namespace scl;

namespace sft
{

static Config &Config::singleton()
{
	static Config cfg;
	return cfg;
}

Config::Config()
{
	config_file rfile(std::string(getenv("HOME")) + "/.sftracker", config_file::READ);

	_searchPaths = rfile.get<std::string>("sf_search_paths");
	
	rfile.close();
}

Config::~Config()
{
	config_file file(std::string(getenv("HOME")) + "/.sftracker", config_file::WRITE);
   
	file.put("sf_search_paths", _searchPaths);

	file.write_changes();

	file.close();
}

}	//namespace sft
