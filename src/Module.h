#pragma once

#include "Instrument.h"
#include "SoundFont.h"
#include "Pattern.h"

namespace sft
{

class Module
{
public:
	Module();

	~Module();

	typedef std::shared_ptr<Module> P;

	static P current();

	static void setCurrent(P module);

	//options

	int bpm() const									{ READ_LOCK; return _bpm; }

	void setBpm(int bpm)							{ WRITE_LOCK; _bpm = bpm; }

	int lpb() const									{ READ_LOCK; return _lpb; }

	void setLpb(int lpb)							{ WRITE_LOCK; _lpb = lpb; }

	int tpl() const									{ READ_LOCK; return _tpl; }

	void setTpl(int tpl)							{ WRITE_LOCK; _tpl = tpl; }

	int tickRate() const							{ READ_LOCK; return _bpm * _lpb * _tpl; }

	//for MIDI
	int ticksPerBeat() const						{ READ_LOCK; return _lpb * _tpl; }

	bool useFlats() const							{ READ_LOCK; return _useFlats; }

	void setUseFlats(bool useFlats)					{ WRITE_LOCK; _useFlats = useFlats; }

	//SoundFonts

	int soundFonts() const 							{ READ_LOCK; return _soundFonts.size(); }

	const SoundFont &soundFont(int index) const		{ READ_LOCK; return _soundFonts[index]; }

	void addSoundFont(const SoundFont &s)			{ WRITE_LOCK; _soundFonts.push_back(s); }

	int removeUnusedSoundFonts();					//returns number of unused SoundFonts

	//instruments

	int instruments() const							{ READ_LOCK; return _instruments.size(); }

	const Instrument &instrument(int index) const	{ READ_LOCK; return _instruments[index]; }

	//tracks (MIDI channels)

	const std::string &trackName(int track) const	{ READ_LOCK; return _tracks[track].name; }

	int noteColumns(int track)						{ READ_LOCK; return _tracks[track].noteColumns; }

	int fxColumns(int track)						{ READ_LOCK; return _tracks[track].fxColumns; }

	int volume(int track)							{ READ_LOCK; return _tracks[track].volume; }

	int pan(int track)								{ READ_LOCK; return _tracks[track].pan; }

	int chorus(int track)							{ READ_LOCK; return _tracks[track].chorus; }

	int reverb(int track)							{ READ_LOCK; return _tracks[track].reverb; }

	void setTrackName(int track,
					  const std::string &name)		{ WRITE_LOCK; _tracks[track].name = name; }

	void setNoteColumns(int track,
						int columns)				{ WRITE_LOCK; _tracks[track].noteColumns = columns; }

	void setFxColumns(int track,
					  int columns)					{ WRITE_LOCK; _tracks[track].fxColumns = columns; }

	void setVolume(int track,
				   int volume)						{ WRITE_LOCK; _tracks[track].volume = volume; }

	void setPan(int track,
				int pan)							{ WRITE_LOCK; _tracks[track].pan = pan; }

	void setChorus(int track,
				   int chorus)						{ WRITE_LOCK; _tracks[track].chorus = chorus; }

	void setReverb(int track,
				   int reverb)						{ WRITE_LOCK; _tracks[track].reverb = reverb; }

	//patterns

	int patterns() const							{ READ_LOCK; return _patterns.size(); }

	Pattern::P pattern(int index) const				{ READ_LOCK; return _patterns[index]; }

	int addPattern();								//returns the new pattern number

	bool deleteEmptyPatterns();						//returns true if deleted

	bool deleteUnusedPatterns();					//this also deletes non-empty patterns that are not in the order

	//playback order

	int orderLength() 								{ READ_LOCK; return _order.size(); }

	int order(int index) const 						{ READ_LOCK; return _order[index].pattern; }

	Pattern::P orderPattern(int order) const		{ READ_LOCK; return _patterns[_order[order].pattern]; }

	int trackMask(int order) const					{ READ_LOCK; return _order[order].trackMask; }

	bool isTrackEnabled(int order,
						int track) const 			{ READ_LOCK; return (_order[order].trackMask & (1 << track)) != 0; }

	void orderInsertBefore();

	void orderInsertAfter();

	void orderDelete();

	void orderSetPattern(int order, int pattern)	{ WRITE_LOCK; _order[order].pattern = pattern; }

	void orderEnableTrack(int track, bool enable);

	//editing position

	int editingOrder() const						{ READ_LOCK; return _editingOrder; }

	void setEditingOrder();

	int editingLine() const							{ READ_LOCK; return _editingLine; }

	void setEditingLine(int pos,
						bool stayInPattern);

	int editingTrack() const						{ READ_LOCK; return _editingTrack; }

	void setEditingTrack(int track);

	int editingColumn() const						{ READ_LOCK; return _editingColumn; }

	void setEditingColumn();

	//tools

	//mutex

	void readLock()									{ _mutex.lock_shared(); }

	void readUnlock()								{ _mutex.unlock_shared(); }

	void writeLock()								{ _mutex.lock(); }

	void writeUnlock()								{ _mutex.unlock(); }

	//serialization

	void serializeOut(std::ostream &os);

	bool serializeIn(std::istream &is);

private:
	static P 					_current;

	std::vector<SoundFont>		_soundFonts;
	std::vector<Instrument>		_instruments;
	std::vector<Pattern::P>		_patterns;

	struct Track
	{
		std::string	name;
		int 		noteColumns;	//1..MAX_NOTE_COLUMNS
		int 		fxColumns;		//1..MAX_FX_COLUMNS
		int 		volume;
		int			pan;
		int			chorus;
		int			reverb;
	};

	Track 						_tracks[TRACK_COUNT];

	struct OrderItem
	{
		int pattern;
		int trackMask;
	};

	std::vector<OrderItem>		_order;

	int 						_editingOrder	= 0;
	int							_editingLine	= 0;
	int							_editingTrack	= 0;
	int							_editingColumn	= 0;

	int							_bpm			= 120;
	int							_lpb			= 4;
	int							_tpl			= 6;
	bool 						_useFlats 		= false;

	mutable std::shared_mutex	_mutex;
};

}	//namespace sft