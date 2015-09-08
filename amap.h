#ifndef __AMAP_H
#define __AMAP_H
#include <map>

namespace awk {

typedef std::map<int, std::string> afield;

class awk_like;
class map {
	public:
		map(awk_like &a);
			
		//Proxy class of std::string(afield::mapped_type)
		class Proxy {
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
				
				//Proxy of char& operator[](size_t pos);
				char &operator[](size_t pos);

				//Proxy of const char& operator[](size_t pos) const;
				const char &operator[](size_t pos) const;
			private:
				map &pm;
				int key;
				
				void recompute();
				void resplit();
				friend std::ostream& operator<<(std::ostream &, const Proxy &);
				friend bool operator<(const Proxy &lhs, const Proxy &rhs);
				friend bool operator<(const Proxy &lhs, const afield::mapped_type &rhs);
				friend bool operator<(const afield::mapped_type &lhs, const Proxy &rhs);
				friend afield::mapped_type operator+(const Proxy &lhs, const Proxy &rhs);
				friend afield::mapped_type operator+(const Proxy &lhs, const afield::mapped_type &rhs);
				friend afield::mapped_type operator+(const afield::mapped_type &lhs, const Proxy &rhs);
		};

		Proxy operator[](int key);
		const Proxy operator[](int key) const;

		void clear();
		size_t size() const;
	private:
		afield _field;
		awk_like &parent;

		friend class Proxy;
		friend bool operator<(const Proxy &lhs, const Proxy &rhs);
		friend bool operator<(const Proxy &lhs, const afield::mapped_type &rhs);
		friend bool operator<(const afield::mapped_type &lhs, const Proxy &rhs);
		friend afield::mapped_type operator+(const Proxy &lhs, const Proxy &rhs);
		friend afield::mapped_type operator+(const Proxy &lhs, const afield::mapped_type &rhs);
		friend afield::mapped_type operator+(const afield::mapped_type &lhs, const Proxy &rhs);

};

};

#endif

