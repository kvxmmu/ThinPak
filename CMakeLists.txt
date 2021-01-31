cmake_minimum_required(VERSION 3.16)
project(ThinPak)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

add_library(thinpak_threads ThinPak/ThreadPool/Queue.cpp ThinPak/ThreadPool/Queue.hpp
        ThinPak/ThreadPool/ThreadPool.cpp ThinPak/ThreadPool/ThreadPool.hpp)
target_link_libraries(thinpak_threads ${CMAKE_THREAD_LIBS_INIT})

add_library(thinpak_aio ThinPak/aio/selector.cpp ThinPak/aio/selector.hpp
        ThinPak/aio/packets_queue.cpp ThinPak/aio/packets_queue.hpp
        ThinPak/aio/event_loop.cpp ThinPak/aio/event_loop.hpp)
