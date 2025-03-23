#include "FluidSynth.h"

#include <fluidsynth/settings.h>

namespace sft
{

FluidSynth &FluidSynth::singleton()
{
	static FluidSynth fs;
	return fs;
}

FluidSynth::FluidSynth()
{
	m_settings 	= new_fluid_settings();
	fluid_settings_setstr(m_settings, "audio.driver", "alsa");

	m_synth 	= new_fluid_synth(m_settings);
	m_adriver 	= new_fluid_audio_driver(m_settings, m_synth);
}

FluidSynth::~FluidSynth()
{
	delete_fluid_audio_driver(m_adriver);
	delete_fluid_synth(m_synth);
	delete_fluid_settings(m_settings);
}

}	//namespace sft
