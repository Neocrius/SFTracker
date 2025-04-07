#pragma once

struct _fluid_synth_t;
struct _fluid_hashtable_t;
struct _fluid_audio_drver_t;
typedef struct _fluid_synth_t fluid_synth_t;
typedef struct _fluid_hashtable_t fluid_settings_t;
typedef struct _fluid_audio_driver_t fluid_audio_driver_t;

namespace sft
{

//FluidSynth instance

class FluidSynth
{
public:
	static FluidSynth &singleton();

	int sfload(const char* path, int resetPresets = 0);

	void sfunload(int id, int resetPresets = 0);

	void programSelect(int channel, int soundFont, int bank, int patch);

	void noteOn(int channel, int key, int velocity);

	void noteOff(int channel, int key);

private:
	FluidSynth();

	~FluidSynth();

	fluid_settings_t 		*_settings;
	fluid_synth_t 			*_synth;
	fluid_audio_driver_t 	*_adriver;
};

}	//namespace sft
