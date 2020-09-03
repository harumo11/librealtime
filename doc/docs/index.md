# librealtime

A modern and extremely easy to use header only C++11 library for realtime processing in Linux.

This library has two different ability classes.
Those thread classes have a interface which is similar as `std::thread`.

- **Realtime thread** : is a real-time periodic thread class.
- **Priority thread** : is a thread class added priority.

You **don't** have to install `Linux real-time patch` or real-time library(e.g. `Xenomal` or `RTAI`).
You can use real-time thread, just install this library.

**NOTE** This library aim making easy-to-use real-time thread class.
If you try mission-critical project(e.g. medical or satellite project), I recomend that you use `Xenomai` or `RTAI`.

## What is a real-time
Real-time means that a function of the thread is called periodicaly and strictly.
real-time ability is useful in research field of system control, robotics and sound.

You can more study regarding to real-time at [Wikipedia](https://en.wikipedia.org/wiki/Real-time_computing)

## What is a priority
Two process is running in one cpu, ability to push away other processes and executed.

You can more study regarding to priority at [Wikipedia](https://en.wikipedia.org/wiki/Nice_(Unix))

## How to use

Real-time and priority thread classes have a very similar interface as `std::thread`.

### Real-time thread

1. Include the header.

	``` c++
	#include <librealtime/RealtimeThread.hpp>
	```

2. Declare the function which you want to call preodicaly in real-time thread.

	``` c++
	// For example, this function is just displaying.
	void func(const int arg){
		// Do great periodic work at here.
		std::cout << arg << std::endl;
	}
	```

3. Declare constructor with arguments(call freequency[Hz], callable function, the function arguments)

	``` c++
	RealtimeThread r_thread(10, func, 1234);
	```

4. Start the thread.

	``` c++
	// If set true, real-time periodic thread will start. 
	// If set false, the periodic thread without real-time ability will start.
	r_thread.start(true);
	```

5. Do other thing until the real-time thread should finish.

	``` c++
	// For example, sleep().
	sleep(3);
	```

6. Finish the real-time thread.

	``` c++
	r_thread.join();
	```

7. The result.
The number should be displied.

	``` c++
	1234
	1234
	1234
	.
	.
	.
	1234
	```

### Policy thread

1. Inlcue the header.
	
	``` c++
	#include <librealtime/PolicyThread.hpp>
	```

2. Declare the function which you want to call with high priolity.
	
	``` c++
	void func(const int num){
		// Do great things here
		std::cout << num << std::endl;
	}
	```

3. Declare constructor with arguments(callable function, the function arguments)

	``` c++
	PolicyThread p_thread(func, 1234);
	```

4. Start the thread.
You can set the priolity of thread with `start()` function. The priolity can take number from 0 to 99 value. priolity 0 means lowest priolity and 99 is highest priolity.
	
	``` c++
	// set the priolity of thread
	p_thread.start(30);
	```

5. Finish the thread.

	``` c++
	p_thread.join();
	```

6. The result.
You can see the number as the result.

	``` c++
	1234
	```
