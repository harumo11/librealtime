#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include "../librealtime/RealtimeThread.hpp"

using namespace std;

void test(std::vector<std::chrono::high_resolution_clock::time_point>& time_points)
{
	time_points.push_back( std::chrono::high_resolution_clock::now());
}

int main(void)
{	
	std::vector<std::chrono::high_resolution_clock::time_point> time_record;
	RealtimeThread th(1000, test, time_record);
	th.start(true);
	sleep(10);
	th.join();

	for(auto &e : time_record){
		cout  << e.time_since_epoch().count() << std::endl;
	}
	
	return 0;
}
