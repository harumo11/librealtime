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
#include "Logger.hpp"

/**
 * @brief A real time thread.
 *	
 * This class make a periodic thread.
 * the thread's loop is invoked with strict frequency.
 * This thread class allows you to use like std::thread.
 */
class PolicyThread {

	protected:
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
		 * @brief If true, break realtime while loop. This is checkd every loop.
		 */
		bool shouldEnd = false;


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
			Logger console;

			sched_param sch;
			pthread_getschedparam(this->thisThread.native_handle(), &this->policy, &sch);
			sch.sched_priority = this->priority;

			if (pthread_setschedparam(this->thisThread.native_handle(), SCHED_FIFO, &sch)) {
				std::cout << std::strerror(errno) << std::endl;
				console.error("faild to setschedparam");
			}
			console.info("Initialization finished");
		}


		/**
		 * @brief Make loop to execute.
		 */
		void makeLoop()
		{
			Logger console;
			console.info("Making Loop");

			while (!this->shouldEnd) {

				this->function();

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
			Logger console;
			console.info("Start constructor");

			this->function = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
			console.info("Store the func");
		}


		/**
		 * @brief Start thread with the priority and FIFO policy.
		 *
		 * @param priority A priority. Set higher number, Get Higher priority. Default priority is 0.
		 */
		void start(int priority = 0)
		{
			this->priority = priority;

			Logger console;
			console.info("Assigned this thread");
			this->started = true;
			this->thisThread = std::thread(&PolicyThread::makeLoop, this);

			//Make thread with priority and policy FIFO
			this->init();
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


		/**
		 * @brief Terminate thread loop.
		 */
		void end()
		{
			Logger console;
			console.info("Thread is end. See you");

			this->shouldEnd = true;
		}


		/**
		 * @brief Get Priority from running thread.
		 *
		 * @return A priority of the thread.
		 */
		int getPriority(){
			pthread_getschedparam(this->thisThread.native_handle(), &this->policy, &this->sch);
			return this->sch.sched_priority;
		}


		/**
		 * @brief A destructor of real time thread.
		 *
		 * Invoke join() or detach().
		 */
		~PolicyThread()
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

