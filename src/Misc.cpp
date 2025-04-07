#include "Misc.h"

namespace sft
{

void writeString(std::ostream &os, const std::string &str)
{
	size_t size = str.size();
	os.write(reinterpret_cast<const char*>(&size), sizeof(size));
	os.write(reinterpret_cast<const char*>(&str[0]), size);
}

bool readString(std::istream &is, OUT std::string *str)
{
	size_t size = 0;
	is.read(reinterpret_cast<char*>(&size), sizeof(size));
	str->resize(size);
	is.read(reinterpret_cast<char*>(&(*str)[0]), size);

	return is.good();
}

std::vector<std::string> split(std::string s, const std::string &delimiter)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}

std::string normalizePath(const std::string &s, bool appendSlash /* = false */)
{
	std::string res;

	for (size_t i = 0; i < s.length(); ++i)
	{
		if (s[i] == '~')
		{
			res = getenv("HOME");
		}
		else if (s[i] == '\\')
		{
			res.append('/', 1);
		}
		else
		{
			res.append(s[i], 1);
		}
	}

	if (appendSlash && res[res.length() - 1] != '/')
	{
		res.append("/", 1);
	}

	return res;
}

std::string fileNameFromPath(const std::string &s)
{
	int slash = s.rfind("/");

	if (slash >= 0)
	{
		return s.substr(slash + 1, s.length() - slash - 1);
	}

	return s;
}

}	//namespace sft
