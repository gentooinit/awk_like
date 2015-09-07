#ifndef __AFUNCTION_H
#define __AFUNCTION_H
#include "amap.h"
#include <regex>

namespace awk {
	
extern afield::mapped_type map_get_ro(const afield &buf, const afield::key_type &key);
extern size_t record_match(std::string str, afield &list, const std::string &sep);
extern std::string replace(const std::string &str, const std::string &rgx, const std::string &fmt);
extern std::string substr(const std::string &str, size_t pos = 0, size_t len = std::string::npos);
extern size_t length(const std::string &str);
extern size_t length(const afield &array);
extern size_t length(const map &array);
extern std::ostream& operator<<(std::ostream &os, const map::Proxy &str);
extern bool operator^(const std::string &str, const char *re);
extern bool operator^(const char *re, const std::string &str);
extern bool operator<(const map::Proxy &lhs, const map::Proxy &rhs);
extern bool operator<(const map::Proxy &lhs, const afield::mapped_type &rhs);
extern bool operator<(const afield::mapped_type &lhs, const map::Proxy &rhs);
extern bool operator<=(const map::Proxy &lhs, const map::Proxy &rhs);
extern bool operator<=(const map::Proxy &lhs, const afield::mapped_type &rhs);
extern bool operator<=(const afield::mapped_type &lhs, const map::Proxy &rhs);
extern bool operator>(const map::Proxy &lhs, const map::Proxy &rhs);
extern bool operator>(const map::Proxy &lhs, const afield::mapped_type &rhs);
extern bool operator>(const afield::mapped_type &lhs, const map::Proxy &rhs);
extern bool operator>=(const map::Proxy &lhs, const map::Proxy &rhs);
extern bool operator>=(const map::Proxy &lhs, const afield::mapped_type &rhs);
extern bool operator>=(const afield::mapped_type &lhs, const map::Proxy &rhs);
extern bool operator==(const map::Proxy &lhs, const map::Proxy &rhs);
extern bool operator==(const map::Proxy &lhs, const afield::mapped_type &rhs);
extern bool operator==(const afield::mapped_type &lhs, const map::Proxy &rhs);
extern bool operator!=(const map::Proxy &lhs, const map::Proxy &rhs);
extern bool operator!=(const map::Proxy &lhs, const afield::mapped_type &rhs);
extern bool operator!=(const afield::mapped_type &lhs, const map::Proxy &rhs);
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

