cmake_minimum_required(VERSION 3.15)
project(neconet)

set(CMAKE_CXX_STANDARD 11)

add_executable(neconet main.cpp base/CThread.cpp base/CThread.h base/CCountDownLatch.cpp base/CCountDownLatch.h base/noncopyable.h base/CMutex.cpp base/CMutex.h base/CurrentThread.h base/CCondition.cpp base/CCondition.h base/CAtomic.h base/CurrentThread.cpp base/CThreadData.cpp base/CThreadData.h base/Types.h base/CException.cpp base/CException.h)