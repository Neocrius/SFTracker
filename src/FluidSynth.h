#pragma once

struct fluid_synth_t;

namespace sft
{

//FluidSynth instance

class FluidSynth
{
public:
	FluidSynth &singleton();

	fluid_synth_t* synth() const	{ return m_synth; }

private:
	FluidSynth();

	~FluidSynth();

	fluid_settings_t 		*m_settings;
	fluid_synth_t 			*m_synth;
	fluid_audio_driver_t 	*m_adriver;
}

}	//namespace sft
