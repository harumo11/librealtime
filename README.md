librealtime
==============
## A modern and extremely easy C++11 library of real time processing.

## 1.Interoduction

This library's target is support to use real-time processing
which is needed system-control field.

This library allows you to make strict periodic task 
without difficult Linux kernel and POSIX API knowledge. 

You can make real-time thread like std::thread.

|Name|Discription|
|----|-----------|
|RealtimeThread|A real-time periodic thread|
|PolicyThread|A Loop thread with priority|

## 2. How to use

```cpp
#include <iostream>
#include <unistd.h>
#include "librealtime/RealtimeThread.hpp"

void func(int num){
	std::cout << "This is Real-Time periodic thread " << num << std::endl;
}

int main(void){
    //Set attribute   (Freequency, function, attribute)
	RealtimeThread rth(10, func, 1234);
	//Run in 10 sec. If set true, get RealTime thread, otherwise, nomal priodic thread.
	rth.start(true);
	sleep(10);
	rth.end();

	//Set attribute  (function, attribute)
	PolicyThread pth(func, 5678);
	//Run with priority 20
	pth.start(20);
	sleep(10);
	pth.end();
}

