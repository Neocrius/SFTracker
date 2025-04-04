#include "Module.h"

namespace sft
{

Module::Module()
{

}

Module::~Module()
{

}

void Module::removeUnusedSoundFonts()
{

}

void Module::orderInsertBefore()
{

}

void Module::orderInsertAfter()
{
	
}

static const uint32_t _magic 	= 'SFTM';
static const uint32_t _fileVer	= 1;

void Module::serializeOut(std::ostream &os)
{
	WRITE(_magic);
	WRITE(_fileVer);

	size_t size = _soundFonts.size();
	WRITE(size);

	for (const auto &sf : _soundFonts)
	{
		sf.serializeOut(os);
	}

	size = _instruments.size();
	WRITE(size);

	for (const auto &inst : _instruments)
	{
		inst.serializeOut(os);
	}	

	size = _patterns.size();
	WRITE(size);

	for (auto pat : _patterns)
	{
		pat->serializeOut(os);
	}	

	size = TRACK_COUNT;
	WRITE(size);
	os.write(reinterpret_cast<const char*>_tracks, sizeof(Track) * TRACK_COUNT);

	size = _order.size();
	WRITE(size);
	os.write(reinterpret_cast<const char*>&_order[0], sizeof(OrderItem) * size);

	WRITE(_playPosition);
	WRITE(_editPosition);
	WRITE(_editingTrack);
	WRITE(_editingColumn);

	WRITE(_bpm);
	WRITE(_lpb);
	WRITE(_tpl);
	WRITE(_useFlats);
}

bool Module::serializeIn(std::istream &is)
{
	uint32_t magic = 0;
	READ(magic);

	if (_magic != magic || is.fail())
	{
		return false;
	}

	uint32_t fileVer = 0;
	READ(fileVer);

	if (_fileVer != fileVer || is.fail())
	{
		return false;
	}

	size_t size = 0;
	READ(size);
	_soundFonts.resize(size);

	for (auto &sf : _soundFonts)
	{
		if (!sf.serializeIn(is))
		{
			//TODO: trigger 'locate SoundFont' dialog
			return false;
		}
	}

	READ(size);
	_instruments.resize(size);

	for (auto &inst : _instruments)
	{
		if (!inst.serializeIn(is))
		{
			return false;
		}
	}	

	READ(size);
	_patterns.resize(size);

	for (size_t i = 0; i < size; ++i)
	{
		_patterns[i] = std::make_shared<Pattern>();

		if (!_patterns[i]->serializeIn(is))
		{
			return false;
		}
	}	

	READ(size);
	if (TRACK_COUNT != size)
	{
		return false;
	}
	os.read(reinterpret_cast<char*>_tracks, sizeof(Track) * 16);

	READ(size);
	_order.resize(size);
	is.read(reinterpret_cast<char*>&_order[0], sizeof(OrderItem) * size);

	READ(_playPosition);
	READ(_editPosition);
	READ(_editingTrack);
	READ(_editingColumn);

	READ(_bpm);
	READ(_lpb);
	READ(_tpl);
	READ(_useFlats);

	return is.good() && is.eof();
}

}	//namespace sft
