#include "FluidSynth.h"

#include <fluidsynth/settings.h>

namespace sft
{

static FluidSynth &FluidSynth::singleton()
{
	static FluidSynth fs;
	return fs;
}

FluidSynth::FluidSynth()
{
	_settings 	= new_fluid_settings();
	fluid_settings_setstr(_settings, "audio.driver", "alsa");

	_synth 	= new_fluid_synth(_settings);
	_adriver 	= new_fluid_audio_driver(_settings, _synth);
}

FluidSynth::~FluidSynth()
{
	delete_fluid_audio_driver(_adriver);
	delete_fluid_synth(_synth);
	delete_fluid_settings(_settings);
}

}	//namespace sft
