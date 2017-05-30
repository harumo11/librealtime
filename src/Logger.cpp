#include <string>
#include <iostream>
#include <sstream>
#include "../include/librealtime/Logger.hpp"

/**
 * @brief Set current hour, minute and second to member value.
 */
void Logger::getCurrentTime()
{
	std::time_t now = std::time(NULL);
	struct tm *pnow = std::localtime(&now);

	this->currentHour = pnow->tm_hour;
	this->currentMin  = pnow->tm_min;
	this->currentSec  = pnow->tm_sec;
}

/**
 * @brief Add color showing information and current time to message argument.
 *
 * @param message Information message which you want to display.
 *
 * @return [ INFO ] [ hour minute sec ] your message
 */
std::string Logger::info(std::string message)
{
	this->getCurrentTime();

	std::cout << this->charColor(32)+this->bold() << "[ INFO  ] "
		<< this->remove() << "[ " << this->currentHour << " " << this->currentMin 
		<< " " << this->currentSec << " ] " << message << std::endl;

	std::stringstream output;
	output << "[ INFO  ] "
		<< "[ " << this->currentHour << " " << this->currentMin 
		<< " " << this->currentSec << " ] " << message;

	return output.str();
}

/**
 * @brief Add color showing warning state and current time to message argument.
 *
 * @param message Warning message which you want to display.
 *
 * @return [ WARN ] [ hour minute sec ] your message
 */
std::string Logger::warn(std::string message)
{
	this->getCurrentTime();

	std::cout << this->charColor(33)+this->bold() << "[ WARN  ] "
		<< this->remove() << "[ " << this->currentHour << " " << this->currentMin 
		<< " " << this->currentSec << " ] " << message << std::endl;

	std::stringstream output;
	output << "[ WARN  ] "
		<< "[ " << this->currentHour << " " << this->currentMin 
		<< " " << this->currentSec << " ] " << message;

	return output.str();
}

/**
 * @brief Add color showing error state and current time to message argument.
 *
 * @param message Error message which you want to display.
 *
 * @return [ ERROR ] [ hour minute sec ] your message
 */
std::string Logger::error(std::string message)
{
	this->getCurrentTime();

	std::cout << this->charColor(31)+this->bold() << "[ ERROR ] "
		<< this->remove() << "[ " << this->currentHour << " " << this->currentMin 
		<< " " << this->currentSec << " ] " << message << std::endl;

	std::stringstream output;
	output << "[ ERROR  ] "
		<< "[ " << this->currentHour << " " << this->currentMin 
		<< " " << this->currentSec << " ] " << message;

	return output.str();
}

/**
 * @brief Add color sowing critical state and current time to message argument.
 *
 * @param message Critical message which you want to display.
 *
 * @return [ CRITICAL ] [ hour minute sec ] your message
 */
std::string Logger::critical(std::string message)
{
	this->getCurrentTime();

	std::cout << this->charColor(33) << "\033[41m" << this->bold() << "[ CRITICAL ]"
		<< this->remove() << " [ " << this->currentHour << " " << this->currentMin 
		<< " " << this->currentSec << " ] " << message << std::endl;

	std::stringstream output;
	output << "[ CRITICAL  ] "
		<< "[ " << this->currentHour << " " << this->currentMin 
		<< " " << this->currentSec << " ] " << message;

	return output.str();
}

/**
 * @brief Add bold effect.
 *
 * @return Escape sequence to add bold charactor effect.
 * 
 * ```
 * std::cout << Logger::bold() << "Good Morning" << std::endl;
 * ```
 * "Good Morning" will be bold.
 */
std::string Logger::bold()
{
	return std::string("\033[1m");
}

/**
 * @brief Remove all effect.
 *
 * @return Escape sequence to remove all effects.
 * 
 */
std::string Logger::remove()
{
	return std::string("\033[0m");
}

/**
 * @brief Add color to charactor or string.
 *
 * @param color A color which you want to effect is express a integer number as below table.
 * 
 * | Color | Number |
 * |:-----:|:------:|
 * | black |30      |
 * | red   |31      |
 * | green |32      |
 * | yellow|33      |
 * | blue  |34      |
 * | perple|35      |
 * | water |36      |
 * | white |37      |
 *
 * @return Escape sequence to add color of charactor
 */
std::string Logger::charColor(int color)
{
	std::stringstream ss;
	ss << "\033[" << color << "m";
	return ss.str();
}
