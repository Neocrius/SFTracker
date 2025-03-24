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

	Module* current() const							{ return m_current; }

	void setCurrent(Module* module)					{ m_current = module; }

	//options

	int bpm() const									{ return m_bpm; }

	void setBpm(int bpm)							{ m_bpm = bpm; }

	int lpb() const									{ return m_lpb; }

	void setLpb(int lpb)							{ m_lpb = lpb; }

	int tpl() const									{ return m_tpl; }

	void setTpl(int tpl)							{ m_tpl = tpl; }

	int tickRate() const							{ return m_bpm * m_lpb * m_tpl; }

	bool useFlats() const							{ return m_useFlats; }

	void setUseFlats(bool useFlats)					{ m_useFlats = useFlats; }

	//SoundFonts

	int soundFonts() const 							{ return m_soundFonts.size(); }

	const SoundFont &soundFont(int index) const		{ return m_soundFonts[index]; }

	void addSoundFont(const SoundFont &s)			{ m_soundFonts.push_back(s); }

	bool removeUnusedSoundFonts();					//returns true if removed

	//instruments

	int instruments() const							{ return m_instruments.size(); }

	const Instrument &instrument(int index) const	{ return m_instruments[index]; }

	//tracks (MIDI channels)

	const std::string &trackName(int track) const	{ return m_tracks[track].name; }

	int noteColumns(int track)						{ return m_tracks[track].noteColumns; }

	int fxColumns(int track)						{ return m_tracks[track].fxColumns; }

	int velocityScale(int track)					{ return m_tracks[track].velocityScale; }

	bool isDrums(int track)							{ return m_tracks[track].drums; }

	void setTrackName(int track,
					  const std::string &name)		{ m_tracks[track].name = name; }

	void setNoteColumns(int track,
						int columns)				{ m_tracks[track].noteColumns = columns; }

	void setFxColumns(int track,
					  int columns)					{ m_tracks[track].fxColumns = columns; }

	void setVelocityScale(int track,
						  int scale)				{ m_tracks[track].velocityScale = scale; }

	void setDrums(int track,
				  bool drums)						{ m_tracks[track].drums = drums; }

	//patterns

	int patterns() const							{ return m_patterns.size(); }

	int addPattern();								//returns the new pattern number

	bool deleteEmptyPatterns();						//returns true if deleted

	bool deleteUnusedPatterns();					//this also deletes non-empty patterns that are not in the order

	//playback order

	int orderLength() 								{ return m_order.size(); }

	int orderPosition() const 						{ return m_orderPosition; }

	int orderPattern() const 						{ return m_order[m_orderPosition].pattern; }

	bool orderIsTrackEnabled(int track) const 		{ return (m_order[m_orderPosition].trackMask & (1 << track)) != 0; }

	void orderInsertBefore();

	void orderInsertAfter();

	void orderDelete();

	void orderSetPattern(int pattern)				{ m_order[m_orderPosition].pattern = pattern; }

	void orderEnableTrack(int track, bool enable);

	//playback and recording/editing

	int playPosition() const 						{ return m_playPosition; }

	void setPlayPosition(int pos,
						 bool loopPattern);

	int editPoisition() const						{ return m_editPosition; }

	void setEditPosition(int pos,
						 bool stayInPattern);

	int editingTrack() const						{ return m_editingTrack; }

	void setEditingTrack(int track);

	int editingColumn() const						{ return m_editingColumn; }

	void setEditingColumn();

	//tools

	//mutex

	//serialization

	void serializeOut(std::ostream &os);

	bool serializeIn(std::istream &is);

private:
	static Module* 			m_current;

	std::vector<SoundFont>	m_soundFonts;
	std::vector<Instrument>	m_instruments;
	std::vector<Pattern>	m_patterns;

	struct Track
	{
		std::string	name;
		int noteColumns;	//1..MAX_NOTE_COLUMNS
		int fxColumns;		//1..MAX_FX_COLUMNS
		int velocityScale;
		bool drums;
	};

	Track 					m_tracks[16];

	struct OrderItem
	{
		int pattern;
		int trackMask;
	};

	std::vector<OrderItem>	m_order;
	int 					m_orderPosition;

	int						m_playPosition;
	int						m_editPosition;
	int						m_editingTrack;
	int						m_editingColumn;

	int						m_bpm		= 96;
	int						m_lpb		= 4;
	int						m_tpl		= 6;
	bool 					m_useFlats 	= false;
};

}	//namespace sft