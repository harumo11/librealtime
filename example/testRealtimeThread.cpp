//
//This simple program is made to evaluate the performance of librealtime.
//
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include "../librealtime/RealtimeThread.hpp"

using namespace std;

//std::vector<std::chrono::high_resolution_clock::time_point> vec(2000);
std::array<std::chrono::high_resolution_clock::time_point, 1000> time_array;
int i = 0;

void test()
{
	auto point = std::chrono::high_resolution_clock::now();
	time_array[i] = point;
	i++;
}

int main(int argc, char const* argv[])
{
	RealtimeThread th(1000, test);
	th.start(true);
	sleep(1);
	th.join();

	cout << "Recode time" << endl;
	std::ofstream file("log.csv");
	
	for(auto &e : time_array){
		file << e.time_since_epoch().count() << std::endl;
		std::cout << e.time_since_epoch().count() << std::endl;
	}
	cout << "[count] : " << i << std::endl;
	file.close();
	
	return 0;
}
