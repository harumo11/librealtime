#include <iostream>
#include <chrono>
#include <unistd.h>
#include <librealtime/PolicyThread.hpp>

using std::cout;
using std::endl;

int i = 0;

void test(){
	for (i = 0; i < 10; i++) {
		auto T = std::chrono::high_resolution_clock::now()+std::chrono::nanoseconds(1000000);
		std::this_thread::sleep_until(T);
	}
}

int main(int argc, char const* argv[])
{

	PolicyThread pth(test);
	pth.start(99);
	sleep(1);
	cout << pth.getPriority() << endl;
	pth.end();
	std::cout << "[count] " << i << std::endl;
	
	return 0;
}
