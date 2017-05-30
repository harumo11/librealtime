#pragma once

#include <iostream>
#include <string>
#include <ctime>

enum charactorColor {
	black = 30,
	red,  
    green, 
    yellow,
    blue , 
    perple,
    water ,
    white ,
};


class Logger {
	
	public:
		std::string info(std::string message);
		std::string warn(std::string message);
		std::string error(std::string message);
		std::string critical(std::string message);
		static std::string bold();
		static std::string charColor(int color);
		static std::string remove();

	private:
		void getCurrentTime();
		int currentHour;
		int currentMin;
		int currentSec;
		enum charactorColor color;
};

