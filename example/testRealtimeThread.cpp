#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include "../librealtime/RealtimeThread.hpp"

using namespace std;

std::vector<std::chrono::high_resolution_clock::time_point> vec(2000);
int i = 0;

void test()
{
	auto point = std::chrono::high_resolution_clock::now();
	vec[i] = point;
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
	
	for(auto &e : vec){
		file << e.time_since_epoch().count() << std::endl;
		cout  << e.time_since_epoch().count() << std::endl;
	}
	cout << "[count] : " << i << std::endl;
	file.close();
	
	return 0;
}
