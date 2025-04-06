#pragma once

#include <iostream>

#define TRACK_COUNT			16
#define MAX_NOTE_COLUMNS	16
#define MAX_FX_COLUMNS		4

namespace sft
{

class Pattern
{
public:
	typedef std::shared_ptr<Pattern> P;
	
	int length() const	{ return _lines.size(); }

	void setLength();

	//reading

	char note(int track, int column, int line) const		{ READ_LOCK; return _lines[line].t[track].notes[column]; }

	char velocity(int track, int column, int line) const	{ READ_LOCK; return _lines[line].t[track].velocities[column]; }

	char fx(int track, int column, int line) const			{ READ_LOCK; return _lines[line].t[track].fx[column]; }

	char parameter(int track, int column, int line) const	{ READ_LOCK; return _lines[line].t[track].parameters[column]; }

	int instrument(int track, int line) const				{ READ_LOCK; return _lines[line].t[track].instrument; }

	//writing

	//tools

	//serialization
	
	void serializeOut(std::ostream &os);

	bool serializeIn(std::istream &is);

private:
	struct TrackLine
	{
		char 	notes[MAX_NOTE_COLUMNS];
		char	velocities[MAX_NOTE_COLUMNS];
		char	fx[MAX_FX_COLUMNS];
		char	parameters[MAX_FX_COLUMNS];
		int 	instrument;		//int for alignment but only goes up to 99
	};

	struct Line
	{
		TrackLine 	t[TRACK_COUNT];
	};

	std::vector<Line> 	_lines;

	std::shared_mutex	_mutex;
};

}	//namespace sft
