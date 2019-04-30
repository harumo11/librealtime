#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include "../librealtime/RealtimeThread.hpp"

// This program show that it is possible to use same as std::thread.
// Particularly, please focus on passing argument.
// You can pass arguments to Realtime and Policy thread class like a std::thread class.

using namespace std;

void test(std::vector<std::chrono::high_resolution_clock::time_point>& time_points)
{
	time_points.push_back( std::chrono::high_resolution_clock::now());
}

int main(void)
{	
	std::vector<std::chrono::high_resolution_clock::time_point> time_record;
	
	// You can pass arguments to Realtime and Policy thread class like a std::thread class.
	RealtimeThread th(100, test, time_record);
	th.start(true);
	sleep(10);
	th.join();

	for(auto &e : time_record){
		cout  << e.time_since_epoch().count() << std::endl;
	}
	
	return 0;
}
