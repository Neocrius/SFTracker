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
	:	_fileName(other._fileName),
		_id(other._id)
	{}

	~SoundFont();

	std::string fileName() const 	{ return _fileName; }

	int id() const 					{ return _id; }	//-1 if not found

private:
	std::string _fileName;
	int 		_id		= -1;
}

}	//namespace sft
