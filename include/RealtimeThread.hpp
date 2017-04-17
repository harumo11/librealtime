#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <functional>
#include <cmath>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "Logger.hpp"

/**
 * @brief Scheduling attributer. It is used to set configuration of realtime
 */
struct sched_attr 
{
	uint32_t size;

	uint32_t sched_policy;
	uint64_t sched_flags;

	//SCHED_FIFO, SCHED_RR
	uint32_t sched_priority;

	//SCHED_DEADLINE
	uint64_t sched_runtime;
	uint64_t sched_deadline;
	uint64_t sched_period;
};

/**
 * @brief A real time thread.
 *	
 * This class make a periodic thread.
 * the thread's loop is invoked with strict frequency.
 * This thread class allows you to use like std::thread.
 */
class RealtimeThread {

	protected:
		/**
		 * @brief period[sec] of thread loop.
		 */
		double periodSecond = 0;					 


		/**
		 * @brief If started, set true, Othrewise false.
		 */
		bool started = false;


		/**
		 * @brief If true, break realtime while loop. This is checkd every loop.
		 */
		bool shouldEnd = false;


		/**
		 * @brief period[nanosec] of thread loop.
		 * 
		 * This variable is converted from periodSecond.
		 */
		unsigned long long periodNonoSecond = 0;


		/**
		 * @brief store the thread task.
		 */
		std::function<void()> function;


		/**
		 * @brief A thread of this class.
		 */
		std::thread thisThread;


		/**
		 * @brief Scheduing attributer. It is used to set configuration of realtime.
		 */
		sched_attr attr;


		/**
		 * @brief Get sched_attr struct.
		 *
		 * Implimation fo sched_getattr() using system call.
		 *
		 * @param pid This process Id / This thread Id
		 * @param attr sched_attr
		 * @param size size of sched_attr
		 * @param flags set 0
		 *
		 * @return If successed, return 0, Otherwise -1. Meaning of error is set in erroro.
		 */
		static int sched_getattr(pid_t pid, struct sched_attr *attr,  unsigned int size, unsigned int flags)
		{
			return syscall(__NR_sched_getattr, pid, attr, size, flags);
		}


		/**
		 * @brief Set sched_attr struct.
		 *
		 * @param pid This proccess Id / This thread Id
		 * @param attr sched_attr
		 * @param flags set 0
		 *
		 * @return If successed. return 0, Otherwise -1. Meaning of error is set in erroro.
		 */
		static int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags)
		{
			return syscall(__NR_sched_setattr, pid, attr, flags);
		}


		/**
		 * @brief Get this thread Id using system call.
		 *
		 * @return This thread Id
		 */
		pid_t gettid(void)
		{
			return syscall(SYS_gettid);
		}


		/**
		 * @brief Set parameters to make real time thread using posix api.
		 */
		void init()
		{
			Logger console;
			console.info("Initialization finished");

			if (this->sched_getattr(this->gettid(), &this->attr, sizeof(this->attr), 0) == -1) {
				std::cout << "[ERROR] Invalid param in sched_getattr() " << std::endl;
			}

			this->attr.size = sizeof(attr);
			uint32_t SCHED_DEADLINE = 6;
			this->attr.sched_policy = SCHED_DEADLINE;
			this->attr.sched_runtime = this->periodNonoSecond;
			this->attr.sched_period  = this->periodNonoSecond;
			this->attr.sched_deadline = this->periodNonoSecond;

			if (this->sched_setattr(this->gettid(), &this->attr, 0) == -1) {
				std::cout << "[ERROR] Invalid param in sched_setattr() " << std::endl;
				std::cout << "Did you execute with super user privilege? " << std::endl;
			}
		}


		/**
		 * @brief Make loop to execute.
		 */
		void makeLoop()
		{
			Logger console;
			console.info("Making Loop");

			auto timePoint = std::chrono::high_resolution_clock::now();
			while (!this->shouldEnd) {
				auto T = std::chrono::high_resolution_clock::now() + std::chrono::nanoseconds(this->periodNonoSecond);

				this->function();

				timePoint = std::chrono::high_resolution_clock::now();
				std::this_thread::sleep_until(T);
			}
		}
	

	public:
		/**
		 * @brief Check if thread is already start.
		 * 
		 * If already started, return true. Othrewise return false.
		 */
		bool isStarted()
		{
			if (this->started) {
				return true;
			}
			else {
				return false;
			}
		}


		/**
		 * @brief A constructor of real time thread class.
		 * 
		 * @param frequency Frequency of executing task of the thread.
		 * @param func Function to execute in thread loop.
		 * @param args Arguments of func. You can set not only one argument but also multiple arguments.
		 */
		template <class F, class... Args>
		RealtimeThread(double frequency, F&& func, Args&&... args)
		{
			Logger console;
			console.info("Start constructor");

			if (frequency <= 0) {
				console.error("Get invalid freequency");
			}
			else {
				console.info("Get valid freequency");
				this->periodSecond = 1/frequency;
				this->periodNonoSecond = static_cast<unsigned long long>(this->periodSecond * std::pow(10, 9));
				std::cout << "period second = " << this->periodSecond << std::endl;
				std::cout << "period nanosecond = " << this->periodNonoSecond << std::endl;
			}

			this->function = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
			console.info("Store the func");
		}


		/**
		 * @brief This method allows to start thread loop.
		 */
		void start(bool shouldRealtime)
		{
			Logger console;
			console.info("Assigned this thread");
			this->started = true;
			this->thisThread = std::thread(&RealtimeThread::makeLoop, this);

			//Make realtime thread using this->thisThread's id.
			if (shouldRealtime) {
				this->init();
			}
		}


		/**
		 * @brief Join the real time thread.
		 *
		 * @return If Join successfully, return true. Otherwise, false.
		 */
		bool join()
		{
			Logger console;

			if (this->isStarted()) {
				try {
					this->thisThread.join();
					console.info("Tread is Joined");
				} catch (const std::system_error& e) {
					std::cout << "Caught system_error with code" << e.code() << " meaning " << e.what() << std::endl;
				}
				return true;
			}
			else {
				console.error("Thread is not yet to run. Can't Join");
				return false;
			}
		}


		/**
		 * @brief Detach the real time thread.
		 *
		 * @return If Detach successfully, return true. Otherwise false.
		 */
		bool detach()
		{
			Logger console;
			if (this->isStarted()) {
				this->thisThread.detach();
				console.info("Thread is Detached");
				return true;
			}
			else {
				console.error("Thread is not yet to run. Can't Detach");
				return false;
			}
		}


		/**
		 * @brief Check the thread can join or not.
		 *
		 * @return If the thread can join, return true. Otherwise false.
		 */
		bool joinable()
		{
			Logger console;
			console.info("Check thread is joinable or not");
			return this->thisThread.joinable();
		}


		void end()
		{
			Logger console;
			console.info("Thread is end. See you");

			this->shouldEnd = true;
		}


		/**
		 * @brief A destructor of real time thread.
		 *
		 * Invoke join() or detach().
		 */
		~RealtimeThread()
		{
			Logger console;

			if (this->joinable()) {
				this->join();
				console.info("Thread is joined in Destructor");
			}
			else {
				this->detach();
				console.info("Thread is detached in Destructor");
			}
		}
};

