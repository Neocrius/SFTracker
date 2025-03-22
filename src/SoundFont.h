#pragma once

#include <string>

namespace sft
{

class SoundFont
{
public:
	SoundFont(std::string &fileName,
			  std::string &searchPaths);

	SoundFont(const SoundFont &other)
	:	m_fileName(other.m_fileName),
		m_id(other.m_id)
	{}

	~SoundFont();

	std::string fileName() const 	{ return m_fileName; }

	int id() const 					{ return m_id; }	//-1 if not found

private:
	std::string m_fileName;
	int 		m_id		= -1;
}

}	//namespace sft
