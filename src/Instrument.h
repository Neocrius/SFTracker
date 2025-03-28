#pragma once

#include <iostream>

namespace sft
{

class Instrument
{
public:
	int soundFont() const				{ return _sf; }

	int bank() const					{ return _bank; }

	int patch() const					{ return _patch; }

	bool drums() const					{ return _drums; }

	void setSoundFont(int sf)			{ _sf = sf; }

	void setBank(int bank)				{ _bank = bank; }

	void setPatch(int patch)			{ _patch = patch; }

	void setDrums(bool b)				{ _drums = b; }

	void programSelect(int channel);

	//serialization
	
	void serializeOut(std::ostream &os);

	bool serializeIn(std::istream &is);

private:
	int		_sf 		= -1;
	int 	_bank		= 0;
	int 	_patch		= 0;
	bool 	_drums		= false;
};

}	//namespace sft