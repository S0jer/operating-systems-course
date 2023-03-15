cmake_minimum_required(VERSION 3.21)
project(Sysopy C)

set(CMAKE_C_STANDARD 99 )

add_executable(santa Zestaw9/zad1/santa.c Zestaw10/zad1/client.c Zestaw10/zad1/server.c Zestaw10/zad1/com.h)
