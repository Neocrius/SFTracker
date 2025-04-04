#include "Instrument.h"
#include "Module.h"
#include "FluidSynth.h"
#include "Misc.h"
#include <fluidsynth/synth.h>

namespace sft
{

void Instrument::programSelect(int channel)
{
	fluid_synth_program_select(
		Synth::singleton()->fs(),
		channel,
		Module::current()->soundFont(_sf).id(),
		_bank,
		_patch);
}

void serializeOut(std::ostream &os)
{
	writeString(os, _name);

	WRITE(_sf);
	WRITE(_bank);
	WRITE(_patch);
	WRITE(_velocity);
	WRITE(_transpose);
	WRITE(_drums);
}

bool serializeIn(std::istream &is)
{
	if (!readString(is, &_name))
		return false;

	READ(_sf);
	READ(_bank);
	READ(_patch);
	READ(_velocity);
	READ(_transpose);
	READ(_drums);

	return is.good();
}

}	//namespace sft
