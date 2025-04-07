#pragma once

#include <shared_mutex>
#include <iostream>
#include <string>

namespace sft
{

#define OUT

#define WRITE(var)	os.write(reinterpret_cast<const char*>(&var), sizeof(var))
#define READ(var)	is.read(reinterpret_cast<char*>(&var), sizeof(var))

//C++14
#define WRITE_LOCK	const std::unique_lock<std::shared_mutex> lock(_mutex)
#define READ_LOCK	const std::shared_lock<std::shared_mutex> lock(_mutex)

void writeString(std::ostream &os, const std::string &str);

bool readString(std::istream &is, OUT std::string *str);

std::vector<std::string> split(std::string s, const std::string &delimiter);

std::string normalizePath(const std::string &s, bool appendSlash = false);

std::string fileNameFromPath(const std::string &s);

}	//namespace sft
