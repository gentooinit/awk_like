#include "awk_like.h"
#include "afunction.h"
#include <regex>

namespace awk {
	
awk_like::awk_like(std::istream &_in,  std::ostream &_out)
	: NR(0), NF(0), 
	RS("\n"), FS("[[:space:]]+"),
	ORS("\n"), OFS(" "),  OFMT("%.6g"),
	field(*this),
	in(_in), out(_out)
{
}

void awk_like::begin() {}
void awk_like::end() {}

void awk_like::print() const
{
	out<<field[0]<<ORS;
}

void awk_like::print(const char *val) const
{
	out<<val<<ORS;
}


void awk_like::loop() {
	exit_flag = false;
	
	std::string buffer;
	std::string last = "";
	
	while (std::getline(in, buffer) && !exit_flag) {
		buffer = last + buffer;
		
		if (!in.eof())
			buffer.push_back('\n');
		
		afield record;
		
		if (!record_match(buffer, record, RS)) {
			//No RS found
			last = buffer;
		} else {
			last = "";

			for (size_t i = 0; i < record.size(); ++i) {
				++NR;
				
				//Disable smart build, don't insert any character into field[0] during split
				smart_build = false;
				NF = split(record[i], field);
				smart_build = true;
				
				each();
			}
		}
	}
	
	if (last != "" && !exit_flag) {
		++NR;

		//Disable smart build, don't insert any character into field[0] during split
		smart_build = false;
		NF = split(last, field);
		smart_build = true;
				
		each();
	}
}

void awk_like::each() 
{
	out<<field[0]<<ORS;	
}

void awk_like::exit()
{
	exit_flag = true;
}

int awk_like::strtonum(const std::string &str)
{
	if (std::regex_match(str, std::regex("[^[:xdigit:]Xx+-]"))) {
		std::cerr<<"error: line: "<<NR<<": Illegal number format."<<std::endl;
		return 0;
	}

	return strtol(str.c_str(), 0, 0);
}


};
