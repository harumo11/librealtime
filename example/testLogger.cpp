#include "Logger.hpp"

using namespace std;

int main(int argc, char const* argv[])
{
	Logger console;
	console.info("thisistestlogger");
	console.error("this is error");
	console.warn("this is warn");
	console.critical("this is critical");
	
	return 0;
}
