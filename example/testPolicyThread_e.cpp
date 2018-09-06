//
//This simple program is made to evaluate the performance of librealtime.
//
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include "../librealtime/PolicyThread.hpp"

using namespace std;

std::vector<std::chrono::high_resolution_clock::time_point> vec(2000);

void test()
{
	int i = 0;
	auto start = std::chrono::high_resolution_clock::now();
	while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start).count() <= 1000) {
	//while(i < 1000){
		vec[i] = std::chrono::high_resolution_clock::now();
		i++;
		usleep(1000);
		//cout << i << "\t" << vec[i].time_since_epoch().count() << endl;
		//cout << "ok_" << endl;
	}
}

int main(int argc, char const* argv[])
{
	PolicyThread th(test);
	cout << "start" << endl;
	th.start(99);
	sleep(1);
	th.join();

	cout << "Record time" << endl;
	std::ofstream file("log.csv");

	for(auto&& e : vec){
		file << e.time_since_epoch().count() << endl;
		cout << e.time_since_epoch().count() << endl;
	}
	cout << "[count] : " << vec.size() << endl;
	file.close();

	return 0;
}
