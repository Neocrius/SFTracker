#include "SoundFont.h"
#include "FluidSynth.h"
#include <synth.h>

namespace sft
{

std::vector<std::string> split(const std::string &s, const std::string &delimiter)
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

std::string normalizePath(const std::string &s, bool appendSlash = false)
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
			res.append('/');
		}
		else
		{
			res.append(s[i]);
		}
	}

	if (appendSlash && res[res.length() - 1] != '/')
	{
		res.append('/');
	}

	return res;
}

std::string fileNameFromPath(const std::string &s)
{
	int slash = rfind(s, '/');

	if (slash >= 0)
	{
		return s.substr(slash + 1, s.length() - slash - 1);
	}

	return s;
}

SoundFont::SoundFont(std::string &fileName,
			  		 std::string &searchPaths)
{
	m_sf = fluid_synth_sfload(FluidSynth::singleton().synth(), normalizePath(fileName).c_str(), 0);

	if (m_sf < 0)
	{
		std::string fileName 			= fileNameFromPath(fileName);
		std::vector<std::string> paths 	= split(searchPaths, ";");

		for (const std::string &path : paths)
		{
			std::string tryFileName = normalizePath(path, true) + fileName;

			m_sf = fluid_synth_sfload(FluidSynth::singleton().synth(), tryFileName.c_str(), 0);

			if (m_sf >= 0)
			{
				m_fileName = tryFileName;
				break;
			}
		}
	}
	else
	{
		m_fileName = fileName;
	}
}

SoundFont::~SoundFont()
{
	if (m_sf >= 0)
	{
		fluid_synth_sfunload(FluidSynth::singleton().synth(), m_sf);
		m_sf = -1;
	}
}

}	//namespace sft
