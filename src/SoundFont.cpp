#include "SoundFont.h"
#include "FluidSynthWrapper.h"
#include "Misc.h"
#include "Config.h"

namespace sft
{

bool SoundFont::load(std::string &fileName)
{
	_id = FluidSynth::singleton().sfload(normalizePath(fileName).c_str());

	if (_id < 0)
	{
		std::string fileName 			= fileNameFromPath(fileName);
		std::vector<std::string> paths 	= split(Config::singleton().searchPaths(), ";");

		for (const std::string &path : paths)
		{
			std::string tryFileName = normalizePath(path, true) + fileName;

			_id = FluidSynth::singleton().sfload(tryFileName.c_str());

			if (_id >= 0)
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
	if (_id >= 0)
	{
		FluidSynth::singleton().sfunload(_id);
		_id = -1;
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
