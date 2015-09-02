#ifndef __AMAP_H
#define __AMAP_H
#include <map>

namespace awk {

typedef std::map<int, std::string> afield;

class awk_like;
class map {
	friend class Proxy;
	public:
		map(awk_like &a);
			
		//Proxy class of std::string(afield::mapped_type)
		class Proxy {
			friend std::ostream& operator<<(std::ostream &, const Proxy &);
			public:
				Proxy(map &m, int k);

				operator afield::mapped_type() const;
				const char *c_str() const;

				//Proxy of string& operator=(const string& str);
				Proxy &operator=(const Proxy &rhs);
				
				Proxy &operator=(afield::mapped_type str);
				
				//Proxy of string& operator=(char c);
				Proxy &operator=(afield::mapped_type::value_type c);
				
				Proxy &operator+=(const Proxy &rhs);
				Proxy &operator+=(afield::mapped_type str);
				bool operator==(const Proxy &rhs) const;
				bool operator==(afield::mapped_type str) const;
				bool operator!=(const Proxy &rhs) const;
				bool operator!=(afield::mapped_type str) const;
			private:
				map &pm;
				int key;
				
				void recompute();
				void resplit();
		};

		Proxy operator[](int key);
		const Proxy operator[](int key) const;

		void clear();
		size_t size();
	private:
		afield _field;
		awk_like &parent;
};

};

#endif

