cmake_minimum_required(VERSION 3.15)
project(neconet)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
add_executable(neconet main.cpp base/CThread.cpp base/CThread.h base/CCountDownLatch.cpp base/CCountDownLatch.h base/noncopyable.h base/CMutex.cpp base/CMutex.h base/CurrentThread.h base/CCondition.cpp base/CCondition.h base/CAtomic.h base/CurrentThread.cpp base/CThreadData.cpp base/CThreadData.h base/Types.h base/CException.cpp base/CException.h base/CTimestamp.cpp base/CTimestamp.h base/copyable.h net/CEventLoop.cpp net/CEventLoop.h net/CChannel.cpp net/CChannel.h net/CPoller.cpp net/CPoller.h net/CTimer.cpp net/CTimer.h net/Callbacks.h net/CTimerId.cpp net/CTimerId.h net/CTimerQueue.cpp net/CTimerQueue.h net/CEventLoopThread.cpp net/CEventLoopThread.h net/CSocket.cpp net/CSocket.h net/CInetAddress.cpp net/CInetAddress.h net/SocketsOps.h net/SocketsOps.cpp)