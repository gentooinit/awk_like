#ifndef __AFUNCTION_H
#define __AFUNCTION_H
#include "amap.h"
#include <regex>

namespace awk {
	
extern size_t record_match(std::string str, afield &list, const std::string &sep);
extern std::string replace(const std::string &str, const std::string &rgx, const std::string &fmt);
extern std::ostream& operator<<(std::ostream &os, const map::Proxy &str);
extern std::string saprintf(const std::string &str, ...);
extern std::string saprintf(const std::string &fmt, const std::string &str);
extern std::string saprintf(const char *fmt);

template<typename T, typename... Args>
inline std::string saprintf(const char *fmt, T val, Args... args)
{
	std::string str;

	//NOTE: '*' trick for width and precision is not supported.
	std::regex speci_rex("%[-+#0 ]{0,1}[0-9]*(\\.[0-9])*((hh)|(ll)|[hljztL])*[diuoxXfFeEgGaAcspn]");
	std::cmatch m;

	while (*fmt) {
		if (*fmt == '%') {
			if (*(fmt + 1) == '%') {
				++fmt;
			} else {
				//Regex match to the specifier
				if (!std::regex_search(fmt, m, speci_rex))
					throw std::runtime_error("invalid format string: missing arguments");

				str += saprintf(m[0], val);
				fmt += m.length();
				str += saprintf(fmt, args...);

				return str;
			}
		}

		str.push_back(*fmt++);
	}

	return str;
}
};
#endif

