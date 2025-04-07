#include "Instrument.h"
#include "Module.h"
#include "FluidSynthWrapper.h"
#include "Misc.h"

namespace sft
{

void Instrument::programSelect(int channel) const
{
	FluidSynth::singleton().programSelect(
		channel,
		Module::current()->soundFont(_sf).id(),
		_bank,
		_patch);
}

void Instrument::serializeOut(std::ostream &os) const
{
	writeString(os, _name);

	WRITE(_sf);
	WRITE(_bank);
	WRITE(_patch);
	WRITE(_velocity);
	WRITE(_transpose);
	WRITE(_drums);
}

bool Instrument::serializeIn(std::istream &is)
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
