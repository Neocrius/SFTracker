#include "Instrument.h"
#include "FluidSynth.h"
#include <fluidsynth/synth.h>

namespace sft
{

void Instrument::programSelect(int channel)
{
	fluid_synth_progra_select(Synth::singleton()->fs(), channel, _sf, _bank, _patch);
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
