#include <iostream>
#include <chrono>
#include "../librealtime/PolicyThread.hpp"

using std::cout;
using std::endl;


void test(){
	int i = 0;
	for (i = 0; i < 10; ) {
		std::cout << "Hi!! I'm thread" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		i++;
	}
}

int main(int argc, char const* argv[])
{

	PolicyThread pth(test);
	pth.start(30);
	pth.join();
	
	return 0;
}
