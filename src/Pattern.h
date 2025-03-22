#pragma once

#include <iostream>

namespace sft
{

class Pattern
{
public:
	int length() const	{ return m_lines.size(); }

	void setLength();

	//reading

	char note(int track, int column, int line) const		{ return m_lines[line].t[track].notes[column]; }

	char velocity(int track, int column, int line) const	{ return m_lines[line].t[track].velocities[column]; }

	char fx(int track, int column, int line) const			{ return m_lines[line].t[track].fx[column]; }

	char parameter(int track, int column, int line) const	{ return m_lines[line].t[track].parameters[column]; }

	int instrument(int track, int line) const				{ return m_lines[line].t[track].instrument; }

	//writing

	//tools

	//serialization
	
	void serializeOut(std::ostream &os);

	bool serializeIn(std::istream &is);

private:
	struct TrackLine
	{
		char 	notes[16];
		char	velocities[16];
		char	fx[4];
		char	parameters[4];
		int 	instrument;		//int for alignment but only goes up to 99
	};

	struct Line
	{
		TrackLine 	t[16];
	};

	std::vector<Line> m_lines;
};

}	//namespace sft
