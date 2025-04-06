#pragma once

struct fluid_synth_t;

namespace sft
{

//FluidSynth instance

class FluidSynth
{
public:
	static FluidSynth &singleton();

	fluid_synth_t* synth() const	{ return _synth; }

	void noteOn(int channel, int key, int velocity);

	void noteOff(int channel, int key);

private:
	FluidSynth();

	~FluidSynth();

	fluid_settings_t 		*_settings;
	fluid_synth_t 			*_synth;
	fluid_audio_driver_t 	*_adriver;
}

}	//namespace sft
