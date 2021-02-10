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

const size_t size = 1000;
std::array<std::chrono::high_resolution_clock::time_point, size> time_array;
int i = 0;

void test()
{
	auto point = std::chrono::high_resolution_clock::now();
	if(i<size) time_array[i] = point;
	i++;
}

int main(int argc, char const* argv[])
{
	RealtimeThread th(size, test);
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
