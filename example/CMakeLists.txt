cmake_minimum_required(VERSION 3.5)

add_definitions("-std=c++11")

add_executable(testPolicyThread testPolicyThread.cpp)
target_link_libraries(testPolicyThread pthread)
