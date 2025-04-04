#include "SoundFont.h"
#include "FluidSynth.h"
#include "Misc.h"
#include <synth.h>

namespace sft
{

bool SoundFont::load(std::string &fileName)
{
	_sf = fluid_synth_sfload(FluidSynth::singleton().synth(), normalizePath(fileName).c_str(), 0);

	if (_sf < 0)
	{
		std::string fileName 			= fileNameFromPath(fileName);
		std::vector<std::string> paths 	= split(Config::singleton().searchPaths(), ";");

		for (const std::string &path : paths)
		{
			std::string tryFileName = normalizePath(path, true) + fileName;

			_sf = fluid_synth_sfload(FluidSynth::singleton().synth(), tryFileName.c_str(), 0);

			if (_sf >= 0)
			{
				_fileName = tryFileName;
				return true;
			}
		}

		return false;
	}
	else
	{
		_fileName = fileName;
	}

	return true;
}

SoundFont::~SoundFont()
{
	if (_sf >= 0)
	{
		fluid_synth_sfunload(FluidSynth::singleton().synth(), _sf);
		_sf = -1;
	}
}

void SoundFont::serializeOut(std::ostream &os) const
{
	writeString(os, _fileName);
}

bool SoundFont::serializeIn(std::istream &is)
{
	if (!readString(is, &_fileName))
	{
		return false;
	}

	return load(_fileName);
}

}	//namespace sft
