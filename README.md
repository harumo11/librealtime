librealtime
==============
## A modern and extremely easy header only C++11 library for real time processing in Linux.

## 1.Introduction

This library's target is support to use real-time processing which is needed robotics and system control field.

This library allows you to make strict periodic tasks
and the tasks with priority without difficult Linux kernel and POSIX API knowledge

You can use librealtime library like std::thread library.
So, you can start to use librealtime quickly.

Web API documentation is [HERE](https://harumo11.github.io/librealtime/)

## 2.Contents

### Library
|Name|Discription|Scheduling|
|----|-----------|----------|
|RealtimeThread|A real-time periodic thread|SCHED_DEADLINE|
|PolicyThread|A std::thread added priority|SCHED_FIFO|

### The API documentation(Doxygen)

- [web api documentation](https://harumo11.github.io/librealtime/)
- local api documentation(librealtime/docs/index.html)

Please open with web browser(firefox, chrome and so on).

If you want to build the API documentation, type following commands.
```
cd librealtime
doxygen Doxyfile
cd html
firefox index.html
```

## 3. How to use

### Realtime Thread

```cpp
#include <iostream>
#include <unistd.h>
#include <librealtime/RealtimeThread.hpp>

void func_real(int num){
	std::cout << "This is Real-Time periodic thread " << num << std::endl;
}

int main(void){
    //(Freequency[Hz], function, arguments)
	RealtimeThread rth(10, func, 1234);
	
	//Run in 10 sec. If set true, get RealTime thread, otherwise, nomal priodic thread.
	rth.start(true);
	sleep(3);
	rth.join();
}
```

### Policy Thread

```cpp
#include <iostream>
#include <librealtime/PolicyThread.hpp>

void func_policy(int num){
	std::cout << "This is Policy-Thread " << num << std::endl;
}

int main(void){

	//set function and arguments
	PolicyThread pth(func_policy, num);
	//(Priority. fron 0 to 99)
	pth.start(30);
	pth.join();
}
```

## 3. Installation

```
git clone https://github.com/harumo11/librealtime.git
cd librealtime
sudo cp -r librealtime /usr/local/include
```

## 4.Dependency

- librealtime can use **ONLY** linux.
	This library was tested on **Ubuntu** and **Linux Mint**.
- librealtime require C++11 or more greater.


## 5.Accuracy

What's happen if realtime thread is made with PolicyThread?
In other words, What's difference between RealtimeThread and PolicyThread?

Ok, Let's conduct a experiment!

Two program are made for experiment. you can find those programs in
**librealtime/example/testRealtimeThread.cpp, PolicyThread.cpp**
Those program try to call certain function 1000 times in 1 second. One program has a thread which is made by RealtimeThread, Onother program has a thread which is made by PolicyThread.

In short, environment of experiment is as below.
- 1000[Hz]
- One thread is RealtimeThread(librealtime/example/testRealtime.cpp)
- Onother thread is PolicyThread(librealtime/example/testPolicyThread_e.cpp)

The results are as below.
![Realtime](./data/realtime_test.png)
![Policy](./data/policy_test.png)

|Thread|Average|Standerd deviation|Counts|
|------|--------|------------------|------|
|RealtimeThread|1000129.986[nano sec]|1452.211[nano sec]|1000/1000 times|
|PolicyThread|1060530.358[nano sec]|12778.745[nano sec]|944/1000 times|
