#pragma once

#include "Instrument.h"
#include "SoundFont.h"

#define MAX_NOTE_COLUMNS	16
#define MAX_FX_COLUMNS		4

namespace sft
{

class Module
{
public:
	Module();

	~Module();

	Module* current() const							{ return _current; }

	void setCurrent(Module* module)					{ _current = module; }

	//options

	int bpm() const									{ return _bpm; }

	void setBpm(int bpm)							{ _bpm = bpm; }

	int lpb() const									{ return _lpb; }

	void setLpb(int lpb)							{ _lpb = lpb; }

	int tpl() const									{ return _tpl; }

	void setTpl(int tpl)							{ _tpl = tpl; }

	int tickRate() const							{ return _bpm * _lpb * _tpl; }

	bool useFlats() const							{ return _useFlats; }

	void setUseFlats(bool useFlats)					{ _useFlats = useFlats; }

	//SoundFonts

	int soundFonts() const 							{ return _soundFonts.size(); }

	const SoundFont &soundFont(int index) const		{ return _soundFonts[index]; }

	void addSoundFont(const SoundFont &s)			{ _soundFonts.push_back(s); }

	bool removeUnusedSoundFonts();					//returns true if removed

	//instruments

	int instruments() const							{ return _instruments.size(); }

	const Instrument &instrument(int index) const	{ return _instruments[index]; }

	//tracks (MIDI channels)

	const std::string &trackName(int track) const	{ return _tracks[track].name; }

	int noteColumns(int track)						{ return _tracks[track].noteColumns; }

	int fxColumns(int track)						{ return _tracks[track].fxColumns; }

	int velocityScale(int track)					{ return _tracks[track].velocityScale; }

	bool isDrums(int track)							{ return _tracks[track].drums; }

	void setTrackName(int track,
					  const std::string &name)		{ _tracks[track].name = name; }

	void setNoteColumns(int track,
						int columns)				{ _tracks[track].noteColumns = columns; }

	void setFxColumns(int track,
					  int columns)					{ _tracks[track].fxColumns = columns; }

	void setVelocityScale(int track,
						  int scale)				{ _tracks[track].velocityScale = scale; }

	void setDrums(int track,
				  bool drums)						{ _tracks[track].drums = drums; }

	//patterns

	int patterns() const							{ return _patterns.size(); }

	int addPattern();								//returns the new pattern number

	bool deleteEmptyPatterns();						//returns true if deleted

	bool deleteUnusedPatterns();					//this also deletes non-empty patterns that are not in the order

	//playback order

	int orderLength() 								{ return _order.size(); }

	int orderPosition() const 						{ return _orderPosition; }

	int orderPattern() const 						{ return _order[_orderPosition].pattern; }

	bool orderIsTrackEnabled(int track) const 		{ return (_order[_orderPosition].trackMask & (1 << track)) != 0; }

	void orderInsertBefore();

	void orderInsertAfter();

	void orderDelete();

	void orderSetPattern(int pattern)				{ _order[_orderPosition].pattern = pattern; }

	void orderEnableTrack(int track, bool enable);

	//playback and recording/editing

	int playPosition() const 						{ return _playPosition; }

	void setPlayPosition(int pos,
						 bool loopPattern);

	int editPoisition() const						{ return _editPosition; }

	void setEditPosition(int pos,
						 bool stayInPattern);

	int editingTrack() const						{ return _editingTrack; }

	void setEditingTrack(int track);

	int editingColumn() const						{ return _editingColumn; }

	void setEditingColumn();

	//tools

	//mutex

	//serialization

	void serializeOut(std::ostream &os);

	bool serializeIn(std::istream &is);

private:
	static Module* 			_current;

	std::vector<SoundFont>	_soundFonts;
	std::vector<Instrument>	_instruments;
	std::vector<Pattern::P>	_patterns;

	struct Track
	{
		std::string	name;
		int noteColumns;	//1..MAX_NOTE_COLUMNS
		int fxColumns;		//1..MAX_FX_COLUMNS
		int velocityScale;
		bool drums;
	};

	Track 					_tracks[16];

	struct OrderItem
	{
		int pattern;
		int trackMask;
	};

	std::vector<OrderItem>	_order;
	int 					_orderPosition;

	int						_playPosition;
	int						_editPosition;
	int						_editingTrack;
	int						_editingColumn;

	int						_bpm		= 96;
	int						_lpb		= 4;
	int						_tpl		= 6;
	bool 					_useFlats 	= false;
};

}	//namespace sft