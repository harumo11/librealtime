cmake_minimum_required(VERSION 3.5)

project(librealtime)

add_definitions("-std=c++11")

set(SRC_DIR src)
set(INCLUDE_DIR include/librealtime)

include_directories(${INCLUDE_DIR})

add_library(realtime STATIC ${SRC_DIR}/Logger.cpp)
target_link_libraries(realtime pthread)

install( TARGETS realtime
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)

install(DIRECTORY include/librealtime
	DESTINATION /usr/local/include
	)
