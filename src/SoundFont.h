#pragma once

#include <string>

namespace sft
{

class SoundFont
{
public:
	~SoundFont();

	bool load(std::string &fileName);

	std::string fileName() const 	{ return _fileName; }

	int id() const 					{ return _id; }	//-1 if not found

	//serialization
	
	void serializeOut(std::ostream &os) const;

	bool serializeIn(std::istream &is);

private:
	std::string _fileName;
	int 		_id		= -1;
};

}	//namespace sft
