#ifndef __AWK_LIKE_H
#define __AWK_LIKE_H
#include <iostream>
#include <string>
#include <map>
#include <regex>

namespace awk {
	
typedef std::map<int, std::string> afield;

class awk_like {
	protected:
		int NR;                                           //Number of Row
		int NF;                                           //Number of Field
		std::string RS;                                   //Row Separator
		std::string FS;                                   //Field Separator
		std::string ORS;                                  //Output Row Separator
		std::string OFS;                                  //Output Field Separator
			
		afield field;
		
		std::string all() {
			int i;
			std::string line = "";
			for (i = 0; i < NF - 1; ++i) {
				line += field[i];
				line += OFS;
			}
			line += field[i];
			
			return line;
		}
		
		virtual void each() {
			out<<all()<<ORS;
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
		
		size_t split(std::string str, afield &list, const std::string &sep) {
			size_t counter;
			std::smatch m;
			std::regex e(sep);
				
			list.clear();
			counter = 0;
			
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
			
			return counter;
		}
		
		int split(const std::string &str, afield &list) { return split(str, list, FS); }
	public:
		awk_like(std::istream &_in = std::cin,  std::ostream &_out = std::cout)
			: NR(0), NF(0), 
			RS("\n"), FS("[[:space:]]+"),
			ORS("\n"), OFS(" "),
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
						
						NF = split(row[i], field);
						
						each();
					}
				}
			}
			
			if (last != "" && !exit_flag) {
				++NR;

				NF = split(last, field);
						
				each();
			}
		}
	
	private:
		std::istream &in;
		std::ostream &out;
		
		bool exit_flag;
		
		size_t row_match(std::string str, afield &list, const std::string &sep) {
			size_t counter;
			std::smatch m;
			std::regex e(sep);

			list.clear();
			counter = 0;
			
			if (sep == "") {
				counter = 0;
			} else {
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

};

#endif

