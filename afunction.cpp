#include "afunction.h"
#include <cstdarg>
#include <cstdio>
#include <memory>
#include <regex>

namespace awk {

afield::mapped_type map_get_ro(const afield &buf, const afield::key_type &key)
{
	auto it = buf.find(key);

	if (it != buf.end())
		return buf.at(key);
	else
		return "";
}
	
size_t record_match(std::string str, afield &list, const std::string &sep) {
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

std::string substr(const std::string &str, size_t pos, size_t len)
{
	return str.substr(pos, len);
}

std::ostream& operator<<(std::ostream &os, const map::Proxy &str)
{
	return (os<<static_cast<afield::mapped_type>(str));
}	

bool operator^(const std::string &str, const char *re)
{
	std::smatch m;

	return std::regex_search(str, m, std::regex(re));
}

bool operator^(const char *re, const std::string &str)
{
	std::smatch m;

	return std::regex_search(str, m, std::regex(re));
}

bool operator<(const map::Proxy &lhs, const map::Proxy &rhs)
{
	return map_get_ro(lhs.pm._field, lhs.key) < map_get_ro(rhs.pm._field, rhs.key);
}

bool operator<(const map::Proxy &lhs, const afield::mapped_type &rhs)
{
	return map_get_ro(lhs.pm._field, lhs.key) < rhs;
}

bool operator<(const afield::mapped_type &lhs, const map::Proxy &rhs)
{
	return lhs < map_get_ro(rhs.pm._field, rhs.key);
}

bool operator<=(const map::Proxy &lhs, const map::Proxy &rhs)
{
	return (lhs < rhs) || (lhs == rhs);
}

bool operator<=(const map::Proxy &lhs, const afield::mapped_type &rhs)
{
	return (lhs < rhs) || (lhs == rhs);
}

bool operator<=(const afield::mapped_type &lhs, const map::Proxy &rhs)
{
	return (lhs < rhs) || (lhs == rhs);
}

bool operator>(const map::Proxy &lhs, const map::Proxy &rhs)
{
	return !(lhs <= rhs);
}

bool operator>(const map::Proxy &lhs, const afield::mapped_type &rhs)
{
	return !(lhs <= rhs);
}

bool operator>(const afield::mapped_type &lhs, const map::Proxy &rhs)
{
	return !(lhs <= rhs);
}

bool operator>=(const map::Proxy &lhs, const map::Proxy &rhs)
{
	return !(lhs < rhs);
}

bool operator>=(const map::Proxy &lhs, const afield::mapped_type &rhs)
{
	return !(lhs < rhs);
}

bool operator>=(const afield::mapped_type &lhs, const map::Proxy &rhs)
{
	return !(lhs < rhs);
}

bool operator==(const map::Proxy &lhs, const map::Proxy &rhs)
{
	return !(lhs < rhs) && !(rhs < lhs);
}

bool operator==(const map::Proxy &lhs, const afield::mapped_type &rhs)
{
	return !(lhs < rhs) && !(rhs < lhs);
}

bool operator==(const afield::mapped_type &lhs, const map::Proxy &rhs)
{
	return !(lhs < rhs) && !(rhs < lhs);
}

bool operator!=(const map::Proxy &lhs, const map::Proxy &rhs)
{
	return !(lhs < rhs) || !(rhs < lhs);
}

bool operator!=(const map::Proxy &lhs, const afield::mapped_type &rhs)
{
	return !(lhs < rhs) || !(rhs < lhs);
}

bool operator!=(const afield::mapped_type &lhs, const map::Proxy &rhs)
{
	return !(lhs < rhs) || !(rhs < lhs);
}

std::string saprintf(const std::string &fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	int len = vsnprintf(nullptr, 0, fmt.c_str(), args) + 1;
	va_end(args);
	std::unique_ptr<char[]> buf(new char[len]);

	va_start(args, fmt);
	vsnprintf(buf.get(), len, fmt.c_str(), args);
	va_end(args);

	return std::string(buf.get(), len - 1);
}

std::string saprintf(const std::string &fmt, const std::string &str)
{
	int len = snprintf(nullptr, 0, fmt.c_str(), str.c_str()) + 1;
	std::unique_ptr<char[]> buf(new char[len]);

	snprintf(buf.get(), len, fmt.c_str(), str.c_str());

	return std::string(buf.get(), len - 1);
}

std::string saprintf(const char *fmt)
{
	std::string str;

	while (*fmt) {
		if (*fmt == '%') {
			if (*(fmt + 1) == '%')
				++fmt;
			else
				throw std::runtime_error("invalid format string: missing arguments");
		}

		str.push_back(*fmt++);
	}
	
	return str;
}



};
