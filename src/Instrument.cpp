#include "Instrument.h"
#include "FluidSynth.h"
#include <fluidsynth/synth.h>

namespace sft
{

void Instrument::programSelect(int channel)
{
	fluid_synth_program_select(Synth::singleton()->fs(), channel, m_sf, m_bank, m_patch);
}

void serializeOut(std::ostream &os)
{
	//TODO
}

bool serializeIn(std::istream &is)
{
	//TODO
	return false;
}

}	//namespace sft
