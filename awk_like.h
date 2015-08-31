#ifndef __AWK_LIKE_H
#define __AWK_LIKE_H
#include <iostream>
#include <string>
#include <regex>
#include "amap.h"

namespace awk {

class awk_like {
	friend class map;
	public:
		awk_like(std::istream &_in = std::cin,  
		         std::ostream &_out = std::cout);
			 	
		void loop();	
		virtual void begin();
		virtual void end();	
	protected:
		int NR;                                           //Number of Record
		int NF;                                           //Number of Field
		std::string RS;                                   //Record Separator
		std::string FS;                                   //Field Separator
		std::string ORS;                                  //Output Record Separator
		std::string OFS;                                  //Output Field Separator
		map field;
		
		template <typename list_t>
		size_t split(std::string str, list_t &list, const std::string &sep)
		{
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
		size_t split(const std::string &str, list_t &list)
		{
			return split(str, list, FS);
		}
		
		void exit();
		int strtonum(const std::string &str);
		
		virtual void each();
	private:
		std::istream &in;
		std::ostream &out;
		
		bool exit_flag;
		bool smart_build;
};


};

#endif

