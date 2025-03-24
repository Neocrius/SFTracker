#pragma once

namespace sft
{

class Config
{
public:
	Config &singleton();

	std::string &soundDriver() const				{ return m_soundDriver; }

	void setSoundDriver(const std::string &driver)	{ m_soundDriver = driver; }

private:
	Config();

	~Config();
}

}	//namespace sft
