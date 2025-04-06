#include "Pattern.h"
#include "Misc.h"

namespace sft
{

void Pattern::serializeOut(std::ostream &os)
{
	READ_LOCK;

	size_t size = _lines.size();
	WRITE(size);

	os.write(reinterpret_cast<const char*>(&_lines[0]), sizeof(Line) * size);
}

bool Pattern::serializeIn(std::istream &is)
{
	WRITE_LOCK;
	
	size_t size = 0;
	READ(size);

	_lines.resize(size);
	is.read(reinterpret_cast<char*>&_lines[0], sizeof(Line) * size);

	return is.good();
}


}	//namespace sft
