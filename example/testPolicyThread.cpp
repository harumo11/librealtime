#include <iostream>
#include <chrono>
#include <unistd.h>
#include "../librealtime/PolicyThread.hpp"

using std::cout;
using std::endl;

int i = 0;

void test(){
	for (i = 0; i < 10; ) {
		std::cout << "Hi!! I'm thread" << std::endl;
		sleep(1);
		i++;
	}
}

int main(int argc, char const* argv[])
{

	PolicyThread pth(test);
	pth.start(20);
	cout << pth.getPriority() << endl;
	//if (pth.joinable()) {
	//	pth.join();
	//}
	std::cout << "[count] " << i << std::endl;
	
	return 0;
}
