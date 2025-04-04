#pragma once

namespace sft
{

#define OUT

#define WRITE(var)	os.write(reinterpret_cast<const char*>&var, sizeof(var))
#define READ(var)	is.read(reinterpret_cast<char*>&var, sizeof(var))

void writeString(std::ostream &os, const std::string &str);

bool readString(std::istream &is, OUT std::string *str);

std::vector<std::string> split(const std::string &s, const std::string &delimiter);

std::string normalizePath(const std::string &s, bool appendSlash = false);

std::string fileNameFromPath(const std::string &s);

}	//namespace sft
