#ifndef __AWK_LIKE_H
#define __AWK_LIKE_H
#include <iostream>
#include <string>
#include <map>
#include <regex>

namespace awk {

typedef std::map<int, std::string> afield;

class map {
	public:
		map(int &n, std::string &fs, bool &flag):
			NF(n), OFS(fs), smart_build(flag) {}
			
		//Proxy class of std::string(afield::mapped_type)
		class Proxy {
			public:
				Proxy(map &m, int k, int &n, std::string &fs, bool &flag)
					: pm(m), key(k), NF(n), OFS(fs), smart_build(flag)  {}

				operator afield::mapped_type() const {
					return pm._field[key];
				}

				//Proxy of string& operator=(const string& str);
				Proxy &operator=(const Proxy &rhs) {
					pm._field[key] = rhs.pm._field[rhs.key];
					
					if (smart_build)
						recompute();
					
					return *this;
				}

				Proxy &operator=(afield::mapped_type str) {
					pm._field[key] = str;
					
					if (smart_build)
						recompute();
					
					return *this;
				}

				//Proxy of string& operator=(char c);
				Proxy &operator=(afield::mapped_type::value_type c) {
					pm._field[key] = c;
					
					if (smart_build)
						recompute();

					return *this;
				}
				
				Proxy &operator+=(const Proxy &rhs) {
					pm._field[key] += rhs.pm._field[rhs.key];
					
					if (smart_build)
						recompute();
					
					return *this;
				}
				
				Proxy &operator+=(afield::mapped_type str) {
					pm._field[key] += str;
					
					if (smart_build)
						recompute();
				
					return *this;
				}
				
				friend std::ostream& operator<<(std::ostream &, const Proxy &);
			private:
				map &pm;
				int key;
				
				int &NF;
				std::string &OFS;
				bool &smart_build;
				
				void recompute() {
					size_t i;
					
					pm._field[0] = "";

					for (i = 1; i <= pm._field.size() - 2; ++i) {
						pm._field[0] += pm._field[i];
						pm._field[0] += OFS;
					}
					pm._field[0] += pm._field[i];
					
					NF = pm._field.size() - 1;
				}
		};

		Proxy operator[](int key) {
			return Proxy(*this, key, NF, OFS, smart_build);
		}

		const Proxy operator[](int key) const {
			return Proxy(const_cast<map &>(*this), key, NF, OFS, smart_build);
		}

		void clear() { _field.clear(); }
		size_t size() { return _field.size() - 1; }
		
		friend class Proxy;
	private:
		afield _field;
		
		int &NF;
		std::string &OFS;
		bool &smart_build;
};

class awk_like {
	protected:
		int NR;                                           //Number of Row
		int NF;                                           //Number of Field
		std::string RS;                                   //Row Separator
		std::string FS;                                   //Field Separator
		std::string ORS;                                  //Output Row Separator
		std::string OFS;                                  //Output Field Separator
		
		map field;
		
		virtual void each() {
			out<<field[0]<<ORS;
		}
		
		void exit() {
			exit_flag = true;
		}
		
		std::string replace(const std::string &str, const std::string &rgx, const std::string &fmt) {
			return regex_replace(str, std::regex(rgx), fmt);
		}
		
		int strtonum(const std::string &str) {
			if (std::regex_match(str, std::regex("[^[:xdigit:]Xx+-]"))) {
				std::cerr<<"error: line: "<<NR<<": Illegal number format."<<std::endl;
				return 0;
			}
			
			return strtol(str.c_str(), 0, 0);
		}
		
		template <typename list_t>
		size_t split(std::string str, list_t &list, const std::string &sep) {
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
		int split(const std::string &str, list_t &list) { return split(str, list, FS); }
	public:
		awk_like(std::istream &_in = std::cin,  std::ostream &_out = std::cout)
			: NR(0), NF(0), 
			RS("\n"), FS("[[:space:]]+"),
			ORS("\n"), OFS(" "), 
			field(NF, OFS, smart_build),
			in(_in), out(_out)
		{
		}
					
		virtual void begin() {}
		virtual void end() {}
		
		void loop() {
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
		
	private:
		std::istream &in;
		std::ostream &out;
		
		bool exit_flag;
		bool smart_build;
		
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
};

inline std::ostream& operator<<(std::ostream &os, 
                        const map::Proxy &str)
{
	return (os<<static_cast<afield::mapped_type>(str));
}

};

#endif

