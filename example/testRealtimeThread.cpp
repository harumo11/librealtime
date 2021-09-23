//
// This simple program is made to evaluate the performance of librealtime.
//
#include "../librealtime/RealtimeThread.hpp"
#include <array>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int i = 0;
const size_t array_size = 10000;
std::chrono::high_resolution_clock::time_point last_time_point;
std::array<double, array_size> time_array;

void test() {
    auto now = std::chrono::high_resolution_clock::now();
    if (i < array_size) {
        double diff =
            std::chrono::duration<double>(now - last_time_point).count();
        last_time_point = now;
        time_array[i] = diff;
    }
    i++;
}

int main(int argc, char const *argv[]) {

    // setting
    last_time_point = std::chrono::high_resolution_clock::now();
    const int freqency = 1000;

    // run realtime thread
    std::cout << "Real time thread will run in 10sec" << std::endl;
    RealtimeThread th(freqency, test);
    th.start(true);

    // wait for 1 second
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // if join() is called, thread try to finish a loop
    th.join();
    std::cout << "Real time thread finished" << std::endl;

    // write result
    cout << "Recode time" << endl;
    std::ofstream file("log.csv");
    for (auto &e : time_array) {
        file << std::setprecision(10) << e << std::endl;
        std::cout << std::setprecision(10) << e << std::endl;
    }
    cout << "[count] : " << i << std::endl;
    file.close();

    return 0;
}
