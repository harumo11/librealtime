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

	private:
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
			if (this->sched_getattr(this->gettid(), &this->attr, sizeof(this->attr), 0) == -1) {
				std::cout << "[ERROR] Invalid param in sched_getattr() " << std::endl;
			}

			this->attr.size = sizeof(attr);
			uint32_t SCHED_DEADLINE = 6;
			this->attr.sched_policy = SCHED_DEADLINE;
			this->attr.sched_runtime = this->periodNonoSecond*0.3;
			this->attr.sched_deadline = this->periodNonoSecond;
			this->attr.sched_period  = this->periodNonoSecond;

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
			auto timePoint = std::chrono::high_resolution_clock::now();
			while (!this->shouldEnd) {
				auto T = std::chrono::high_resolution_clock::now() + std::chrono::nanoseconds(this->periodNonoSecond);

				this->function();

				timePoint = std::chrono::high_resolution_clock::now();

				//sleep until next waking time.
				while (T >= std::chrono::high_resolution_clock::now()) {
				}
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
		 * @param frequency Frequency of executing task of the thread [Hz].
		 * @param func Function to execute in thread loop.
		 * @param args Arguments of func. You can set not only one argument but also multiple arguments.
		 */
		template <class F, class... Args>
		RealtimeThread(double frequency, F&& func, Args&&... args)
		{
			if (frequency <= 0) {
				std::cout << "[RealtimeThread] Get invalid freequency" << std::endl;
			}
			else {
				this->periodSecond = (double)1/frequency;
				this->periodNonoSecond = static_cast<unsigned long long>(this->periodSecond * std::pow(10, 9));
				std::cout << "period second = " << this->periodSecond << std::endl;
				std::cout << "period nanosecond = " << this->periodNonoSecond << std::endl;
			}

			this->function = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
		}


		/**
		 * @brief This method allows to start thread loop.
		 *
		 * @param shouldRealtime If true, this thread works as real time thread. otherwize false is set, this thread works as periodic std::thread.
		 *
		 * Starting your thread with DEADLINE policy which is a real time thread controlling method.
		 */
		void start(bool shouldRealtime)
		{
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
		 * 
		 * Only std::thread::join() will be called inside RealtimeThread::join().
		 *
		 * \NOTE If you want to terminate this thread simply, use end().
		 * Because this function just calls std::thread::join(),
		 * you need to send signal to the thread somehow before you use this function.
		 * otherwise, this function continues to block, or never comes back.
		 */
		bool join()
		{
			if (this->isStarted()) {
				try {
					this->thisThread.join();
				} catch (const std::system_error& e) {
					std::cout << "Caught system_error with code" << e.code() << " meaning " << e.what() << std::endl;
				}
				return true;
			}
			else {
				std::cout << "[Thread is not yet to run. Can't Join" << std::endl;
				return false;
			}
		}


		/**
		 * @brief Detach the real time thread.
		 *
		 * @return If detachment will be successfully, return true. Otherwise false.
		 *
		 *  std::thread::detach() is called in detach() in this function.
		 *  
		 * \NOTE Do not use detach() in order to terminate RealtimeThread.
		 *       Please use end() or join().
		 */
		bool detach()
		{
			if (this->isStarted()) {
				this->thisThread.detach();
				return true;
			}
			else {
				std::cout << "[Thread is not yet to run. Can't Detach" << std::endl;

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
			return this->thisThread.joinable();
		}


		/**
		  * @brief This thread will terminate before the thread will be called.
		  * 
		  * After end() is called, either join(), or detach() will be called appropriately in ~RealtimeThread().
		  */
		void end()
		{
			this->shouldEnd = true;
		}

};

