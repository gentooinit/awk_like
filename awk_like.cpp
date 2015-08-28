#include "awk_like.h"
#include "afunction.h"
#include <regex>

namespace awk {
	
awk_like::awk_like(std::istream &_in,  std::ostream &_out)
	: NR(0), NF(0), 
	RS("\n"), FS("[[:space:]]+"),
	ORS("\n"), OFS(" "), 
	field(*this),
	in(_in), out(_out)
{
}

void awk_like::begin() {}
void awk_like::end() {}

void awk_like::loop() {
	exit_flag = false;
	
	std::string buffer;
	std::string last = "";
	
	while (std::getline(in, buffer) && !exit_flag) {
		buffer = last + buffer;
		
		if (!in.eof())
			buffer.push_back('\n');
		
		afield row;
		
		if (!row_match(buffer, row, RS)) {
			//No RS found
			last = buffer;
		} else {
			last = "";

			for (size_t i = 0; i < row.size(); ++i) {
				++NR;
				
				//Disable smart build, don't insert any character into field[0] during split
				smart_build = false;
				NF = split(row[i], field);
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

template <typename list_t>
size_t awk_like::split(std::string str, list_t &list, const std::string &sep)
{
	size_t counter;
	std::smatch m;
	std::regex e(sep);
	
	list.clear();

	//Original str
	list[0] = str;
	
	counter = 1;
	if (sep == "") {
		//NULL separator, separate each character into the list
		for (; counter < str.size(); ++counter)
			list[counter] = str[counter];
	} else {
		while (std::regex_search(str, m, e)) {
			list[counter++] = m.prefix().str();
			str = m.suffix().str();                       //get the rest of str
		}
		list[counter++] = str;                                //get the last part
	}
	
	//Size exclude original str
	return counter - 1;
}

template <typename list_t>
size_t awk_like::split(const std::string &str, list_t &list)
{
	return split(str, list, FS);
}

};
