#include "FluidSynthWrapper.h"
#include <fluidsynth.h>

namespace sft
{

FluidSynth &FluidSynth::singleton()
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

int FluidSynth::sfload(const char* path, int resetPresets /* = 0 */)
{
	return fluid_synth_sfload(_synth, path, resetPresets);
}

void FluidSynth::sfunload(int id, int resetPresets /* = 0 */)
{
	fluid_synth_sfunload(_synth, id, resetPresets);
}

void FluidSynth::programSelect(int channel, int soundFont, int bank, int patch)
{
	fluid_synth_program_select(_synth, channel, soundFont, bank, patch);
}

void FluidSynth::noteOn(int channel, int key, int velocity)
{
	fluid_synth_noteon(_synth, channel, key, velocity);
}

void FluidSynth::noteOff(int channel, int key)
{
	fluid_synth_noteoff(_synth, channel, key);
}

}	//namespace sft
