#pragma once

#include "SoundFont.h"

namespace sft
{

class Instrument
{
public:
	int soundFont() const				{ return m_sf; }

	int bank() const					{ return m_bank; }

	int patch() const					{ return m_patch; }

	void setSoundFont(int sf)			{ m_sf = sf; }

	void setBank(int bank)				{ m_bank = bank; }

	void setPatch(int patch)			{ m_patch = patch; }

	void programSelect(int channel);

	//serialization
	void serializeOut(std::ostream &os);

	bool serializeIn(std::istream &is);

private:
	int	m_sf 	= -1;
	int m_bank	= 0;
	int m_patch	= 0;
};

}	//namespace sft