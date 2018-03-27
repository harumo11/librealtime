#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <functional>
#include <cmath>
#include <mutex>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/types.h>

/**
 * @brief A real time thread.
 *	
 * This class make a periodic thread.
 * the thread's loop is invoked with strict frequency.
 * This thread class allows you to use like std::thread.
 */
class PolicyThread {

	private:
		/**
		 * @brief If started, set true, Othrewise false.
		 */
		bool started = false;


		/**
		 * @brief A policy param
		 */
		int policy;

		/**
		 * @brief A priority param
		 */
		int priority = 0;


		/**
		 * @brief store the thread task.
		 */
		std::function<void()> function;


		/**
		 * @brief A thread of this class.
		 */
		std::thread thisThread;


		/**
		 * @brief Set parameters to make real time thread using posix api.
		 */
		void init()
		{
			sched_param sch;
			pthread_getschedparam(this->thisThread.native_handle(), &this->policy, &sch);
			sch.sched_priority = this->priority;

			if (pthread_setschedparam(this->thisThread.native_handle(), SCHED_FIFO, &sch) != 0) {
				std::cout << "[PolicyThread] faild to setschedparam |\t";
				std::cout << std::strerror(errno) << std::endl;
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
		PolicyThread(F&& func, Args&&... args)
		{
			this->function = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
		}


		/**
		 * @brief Start thread with the priority and FIFO policy.
		 *
		 * @param priority A priority. Set higher number, Get Higher priority. Default priority is 0.
		 * 
		 * Starting your thread with FIFO policy with the priority.
		 * 
		 * - Maximum priority = 99
		 * - Minimum priority = 0
		 */
		void start(int priority = 0)
		{
			this->priority = priority;

			this->started = true;
			//this->thisThread = std::thread(&PolicyThread::function, this);
			this->thisThread = std::thread(this->function);
			std::cout << "thread start" << std::endl;

			//Make thread with priority and policy FIFO
			this->init();
		}

		/**
		 * @brief Get Priority from running thread.
		 *
		 * @return A priority of the thread.
		 */
		int getPriority(){
			return this->priority;
		}

		/**
		 * @brief Join the real time thread.
		 *
		 * Only std::thread::join() will be called in this function.
		 *
		 */
		void join()
		{

			if (this->isStarted()) {
			  	this->thisThread.join();
			}
			else {
			 	std::cout << "[PolicyThread] Thread is not yet to run. Can't Join" << std::endl;
			}
		}


		/**
		 * @brief Detach the real time thread.
		 *
		 * Just std::thread::detach() is called in this function. 
		 * 
		 */
		void detach()
		{
			if (this->isStarted()) {
				this->thisThread.detach();
			}
			else {
				std::cout << "[PolicyThread] Thread is not yet to run. Can't Detach." << std::endl;
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

};

