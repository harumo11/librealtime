#include <iostream>
#include <unistd.h>
#include <librealtime/PolicyThread.hpp>

using std::cout;
using std::endl;

void test(){
	cout << "hello world" << endl;
	sleep(1);
}

int main(int argc, char const* argv[])
{

	PolicyThread pth(test);
	pth.start(20);
	sleep(4);
	cout << pth.getPriority() << endl;
	pth.end();
	
	return 0;
}
