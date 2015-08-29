#ifndef __AFUNCTION_H
#define __AFUNCTION_H
#include "amap.h"

namespace awk {
	
extern size_t record_match(std::string str, afield &list, const std::string &sep);
extern std::string replace(const std::string &str, const std::string &rgx, const std::string &fmt);
extern std::ostream& operator<<(std::ostream &os, const map::Proxy &str);
extern std::string saprintf(const char *fmt, ...);

};
#endif

