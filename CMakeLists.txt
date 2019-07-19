cmake_minimum_required(VERSION 2.4)
project(nvindex)

#set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -DPERF_LATENCY -DUSE_NVM_MALLOC -DNO_CONCURRENT -DNDEBUG -g -O2 -mrtm")
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}  -DUSE_NVM_MALLOC -DCLEAR_NVM_POOL -g -O2 -mrtm")

link_directories("${PROJECT_SOURCE_DIR}/third-party-lib")
link_libraries("${PROJECT_SOURCE_DIR}/third-party-lib/libtbb.so.2")
link_directories(/usr/lib/x86_64-linux-gnu/)
link_libraries(boost_system boost_thread pthread gtest)

include_directories("third-party-lib")
include_directories("include")
include_directories("test")

add_executable(benchmark
		src/main.cpp
		src/util.cpp
		src/threadinfo.cpp
		src/nvm_mgr.cpp
		)

add_executable(recover_test
	src/recover_test.cpp
	src/util.cpp
	src/threadinfo.cpp
	src/nvm_mgr.cpp
	)

add_executable(unittest
    test/unittest.cpp
	src/util.cpp
	src/threadinfo.cpp
	src/nvm_mgr.cpp
)

#add_executable(datagen src/data_generator.cpp)
#add_executable(test_timer benchmarks/testtimer.cpp)
