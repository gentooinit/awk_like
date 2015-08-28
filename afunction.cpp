#include "afunction.h"
#include <regex>

namespace awk {
	
size_t row_match(std::string str, afield &list, const std::string &sep) {
	size_t counter;
	std::smatch m;
	std::regex e(sep);

	list.clear();
	counter = 0;
	
	if (sep != "") {
		while (std::regex_search(str, m, e)) {		
			if (m.prefix().str() != "")
				list[counter++] = m.prefix().str();
				
			str = m.suffix().str();                       //get the rest of str
		}

		if (str != "")
			list[counter++] = str;                        //get the last part
	}
	
	return counter;
}

std::string replace(const std::string &str, const std::string &rgx, const std::string &fmt)
{
	return regex_replace(str, std::regex(rgx), fmt);
}

std::ostream& operator<<(std::ostream &os, const map::Proxy &str)
{
	return (os<<static_cast<afield::mapped_type>(str));
}	

};
