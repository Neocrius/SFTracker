#include "Config.h"
#include <SCL.h>

using namespace scl;

namespace sft
{

Config &Config::singleton()
{
	static Config cfg;
	return cfg;
}

Config::Config()
{
	config_file rfile(std::string(getenv("HOME")) + "/.sftracker", config_file::READ);

	m_soundDriver = rfile.get<std::string>("sound_driver");
	
	rfile.close();
}

Config::~Config()
{
	config_file file(std::string(getenv("HOME")) + "/.sftracker", config_file::WRITE);
   
	file.put("sound_driver", m_soundDriver);

	file.write_changes();

	file.close();
}

}	//namespace sft
