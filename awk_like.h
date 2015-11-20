#ifndef __AWK_LIKE_H
#define __AWK_LIKE_H
#include <iostream>
#include <string>
#include <regex>
#include "amap.h"
#include "afunction.h"

namespace awk {

class awk_like {
	public:
		awk_like(std::istream &_in = std::cin,  
		         std::ostream &_out = std::cout);

		virtual ~awk_like();
			 	
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
		std::string OFMT;                                 //Output Format
		map field;
		static int IGNORECASE;                            //Controls the case-sensitivity of all regex
		
		template <typename list_t>
		size_t split(std::string str, list_t &list, const std::string &sep) const
		{
			auto flag = std::regex::ECMAScript;

			if (IGNORECASE)
				flag |= std::regex::icase;

			std::regex e(sep.c_str(), flag);
			std::smatch m;
			size_t counter;

			list.clear();

			//Original str
			list[0] = str;
			
			counter = 1;
			if (sep == "") {
				//NULL separator, separate each character into the list
				for (; counter <= str.size(); ++counter)
					list[counter] = str[counter - 1];
			} else if (str != "") {
				while (std::regex_search(str, m, e)) {
					list[counter++] = m.prefix().str();
					str = m.suffix().str();                       //get the rest of str
					
					if (str == "")
						break;                                //get out of the loop in case of sep is an empty accepted regex
				}
				list[counter++] = str;                                //get the last part
			}
			
			//Size exclude original str
			return counter - 1;
		}

		template <typename list_t>
		size_t split(const std::string &str, list_t &list) const
		{
			return split(str, list, FS);
		}

		void print() const;
		void print(const char *val) const;

		template <typename T>
		void print(const T &val) const
		{
			if (typeid(T) == typeid(float) || typeid(T) == typeid(double))
				out<<saprintf(OFMT, val);
			else
				out<<val;

			out<<ORS;
		}

		template <typename... Args>
		void print(const char *val, const Args... args) const
		{
			out<<val;
			out<<OFS;
			print(args...);
		}

		template <typename T, typename... Args>
		void print(const T &val, const Args... args) const
		{
			if (typeid(T) == typeid(float) || typeid(T) == typeid(double))
				out<<saprintf(OFMT, val);
			else
				out<<val;

			out<<OFS;
			print(args...);
		}

		void aprintf(const char *fmt) const
		{
			out<<saprintf(fmt);
		};

		template<typename... Args>
		void aprintf(const char *fmt, Args... args) const
		{
			out<<saprintf(fmt, args...);
		};
		
		void exit();
		unsigned long strtonum(const std::string &str);
		
		virtual void each();
	private:
		std::istream &in;
		std::ostream &out;
		
		bool exit_flag;
		bool smart_build;

		friend class map;
		friend std::string replace(const std::string &str, const std::string &rgx, const std::string &fmt);
		friend size_t record_match(std::string str, afield &list, const std::string &sep);
		friend bool operator^(const std::string &str, const char *re);
};


};

#endif

