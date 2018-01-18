#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include "../librealtime/RealtimeThread.hpp"

using namespace std;

std::vector<std::chrono::high_resolution_clock::time_point> vec;

void test()
{
	std::chrono::high_resolution_clock::time_point point;
	point = std::chrono::high_resolution_clock::now();
	//std::cout << "Execute" << std::endl;
	//std::cout << point.time_since_epoch().count() << std::endl;
	vec.push_back(point);
}

int main(int argc, char const* argv[])
{
	RealtimeThread th(100, test);
	th.start(true);
	sleep(1);
	th.end();

	cout << "Recode time" << endl;
	std::ofstream file("log.csv");
	
	int i = 0;
	for(auto &e : vec){
		file << e.time_since_epoch().count() << std::endl;
		cout  << e.time_since_epoch().count() << std::endl;
		i++;
	}
	cout << "[count] : " << i << std::endl;
	file.close();
	
	return 0;
}
