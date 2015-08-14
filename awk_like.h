#ifndef __AWK_LIKE_H
#define __AWK_LIKE_H
#include <iostream>
#include <string>
#include <map>
#include <regex>

typedef std::map<int, std::string> afield;

class awk_like {
	protected:
		int NR;                                           //Number of Row
		int NF;                                           //Number of Field
		char RS;                                          //Row Separator
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
			std::cout<<all()<<ORS;
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
		awk_like(): NR(0), NF(0), 
		            RS('\n'), FS("[[:space:]]"),
			    ORS("\n"), OFS(" ") {}
					
		virtual void begin() {}
		virtual void end() {}
		
		void loop() {
			exit_flag = false;
			while (std::getline(std::cin, line, RS) && !exit_flag) {
				++NR;
				
				NF = split(line, field);
				
				each();
			}
		}
	
	private:
		std::string line;
		bool exit_flag;
};

#endif

