#include "amap.h"
#include "awk_like.h"

namespace awk {
	
map::map(awk_like &a): parent(a) {}

map::Proxy map::operator[](int key)
{
	return Proxy(*this, key);
}

const map::Proxy map::operator[](int key) const
{
	return Proxy(const_cast<map &>(*this), key);
}

void map::clear()
{
	_field.clear(); 
}

size_t map::size()
{
	return _field.size() - 1;
}


map::Proxy::Proxy(map &m, int k): pm(m), key(k) {}

map::Proxy::operator afield::mapped_type() const
{
	return map_get_ro(pm._field, key);
}

const char* map::Proxy::c_str() const
{
	return map_get_ro(pm._field, key).c_str();
}

//Proxy of string& operator=(const string& str);
map::Proxy&
map::Proxy::operator=(const Proxy &rhs)
{
	pm._field[key] = map_get_ro(rhs.pm._field, rhs.key);
	
	if (key == 0) {
		resplit();
	} else if (pm.parent.smart_build) {
		recompute();
	}
	
	return *this;
}

map::Proxy&
map::Proxy::operator=(afield::mapped_type str)
{
	pm._field[key] = str;
	
	if (key == 0) {
		resplit();
	} else if (pm.parent.smart_build) {
		recompute();
	}
	
	return *this;
}

//Proxy of string& operator=(char c);
map::Proxy&
map::Proxy::operator=(afield::mapped_type::value_type c)
{
	pm._field[key] = c;
	
	if (key == 0) {
		resplit();
	} else if (pm.parent.smart_build) {
		recompute();
	}

	return *this;
}

map::Proxy&
map::Proxy::operator+=(const Proxy &rhs)
{
	pm._field[key] += map_get_ro(rhs.pm._field, rhs.key);
	
	if (pm.parent.smart_build)
		recompute();
	
	return *this;
}

map::Proxy&
map::Proxy::operator+=(afield::mapped_type str)
{
	pm._field[key] += str;
	
	if (pm.parent.smart_build)
		recompute();

	return *this;
}

void map::Proxy::recompute()
{
	int i;
	auto tail = pm._field.crbegin();
	
	// The last element of ordered map is always the largest one.
	// Maybe fill the *holes* inside the map with string("")
	pm._field[0] = "";
	for (i = 1; i < tail->first; ++i) {
		pm._field[0] += pm._field[i];
		pm._field[0] += pm.parent.OFS;
	}
	pm._field[0] += pm._field[i];

	pm.parent.NF = pm._field.size() - 1;
}

void map::Proxy::resplit()
{
	pm.parent.NF = pm.parent.split(pm._field[0], pm._field);
}

};


