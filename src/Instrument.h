#pragma once

#include <iostream>

namespace sft
{

class Instrument
{
public:
	int soundFont() const				{ return m_sf; }

	int bank() const					{ return m_bank; }

	int patch() const					{ return m_patch; }

	bool drums() const					{ return m_drums; }

	void setSoundFont(int sf)			{ m_sf = sf; }

	void setBank(int bank)				{ m_bank = bank; }

	void setPatch(int patch)			{ m_patch = patch; }

	void setDrums(bool b)				{ m_drums = b; }

	void programSelect(int channel);

	//serialization
	void serializeOut(std::ostream &os);

	bool serializeIn(std::istream &is);

private:
	int	m_sf 		= -1;
	int m_bank		= 0;
	int m_patch		= 0;
	bool m_drums	= false;
};

}	//namespace sft