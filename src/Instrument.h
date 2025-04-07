#pragma once

#include <iostream>

namespace sft
{

class Instrument
{
public:
	const std::string &name()			{ return _name; }

	int soundFont() const				{ return _sf; }

	int bank() const					{ return _bank; }

	int patch() const					{ return _patch; }

	int velocity() const				{ return _velocity; }

	int transpose() const				{ return _transpose; }

	bool drums() const					{ return _drums; }

	void setName(const std::string &s)	{ _name = s; }

	void setSoundFont(int sf)			{ _sf = sf; }

	void setBank(int bank)				{ _bank = bank; }

	void setPatch(int patch)			{ _patch = patch; }

	void setVelocity(int velocity)		{ _velocity = velocity; }

	void setTranspose(int transpose)	{ _transpose = transpose; }

	void setDrums(bool b)				{ _drums = b; }

	void programSelect(int channel) const;

	//serialization
	
	void serializeOut(std::ostream &os) const;

	bool serializeIn(std::istream &is);

private:
	std::string _name;
	int			_sf 		= -1;
	int 		_bank		= 0;
	int 		_patch		= 0;
	int 		_velocity	= 100;
	int			_transpose	= 0;
	bool 		_drums		= false;
};

}	//namespace sft