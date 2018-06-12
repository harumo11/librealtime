cmake_minimum_required(VERSION 3.5)

add_definitions("-std=c++11")

add_executable(testRealtimeThread testRealtimeThread.cpp)
add_executable(tutorial_realtime_thread tutorial_realtime_thread.cpp)
add_executable(testPolicyThread testPolicyThread.cpp)
add_executable(testPolicyThread_e testPolicyThread_e.cpp)

target_link_libraries(testRealtimeThread pthread)
target_link_libraries(testPolicyThread pthread)
target_link_libraries(testPolicyThread_e pthread)
target_link_libraries(tutorial_realtime_thread pthread)

